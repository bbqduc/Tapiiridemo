#include "oclfile.h"
#include <utility>
#include <cstdlib>
#include <ctime>

#if defined __APPLE__ || defined(MACOSX)
#else
#if defined WIN32
#else
//needed for context sharing functions
#include <GL/glx.h>
#endif
#endif

void displayPlatformInfo(cl::vector< cl::Platform > platformList,
	int deviceType)
{
	// print out some device specific information
	std::cout << "Platform number is: " << platformList.size() << std::endl;

	std::string platformVendor;
	platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, 
		&platformVendor);

	std::cout << "device Type " 
		<< ((deviceType==CL_DEVICE_TYPE_GPU)?"GPU":"CPU") << std::endl;
	std::cout << "Platform is by: " << platformVendor << "\n";
}

void OCLProg::initCL()
{
	try {
		cl::vector< cl::Platform > platformList;
		cl::Platform::get(&platformList);

		displayPlatformInfo(platformList, CL_DEVICE_TYPE_GPU);

#if defined (__APPLE__) || defined(MACOSX)
		CGLContextObj kCGLContext = CGLGetCurrentContext();
		CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
		cl_context_properties props[] =
		{
			CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup,
			0
		};
		//Apple's implementation is weird, and the default values assumed by cl.hpp don't work
		//this works
		//cl_context cxGPUContext = clCreateContext(props, 0, 0, NULL, NULL, &err);
		//these dont
		//cl_context cxGPUContext = clCreateContext(props, 1,(cl_device_id*)&devices.front(), NULL, NULL, &err);
		//cl_context cxGPUContext = clCreateContextFromType(props, CL_DEVICE_TYPE_GPU, NULL, NULL, &err);
		//printf("error? %s\n", oclErrorString(err));
		try{
			context = cl::Context(props);   //had to edit line 1448 of cl.hpp to add this constructor
		}
		catch (cl::Error er) {
			std::cerr << er.what() << '\n';
		}
#else
#if defined WIN32 // Win32
		cl_context_properties props[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(),
			0
		};
		//cl_context cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &err);
		try{
			context = cl::Context(CL_DEVICE_TYPE_GPU, props);
		}
		catch (cl::Error er) {
			std::cerr << er.what() << '\n';
		}
#else
		cl_context_properties props[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
			CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(),
			0
		};
		//cl_context cxGPUContext = clCreateContext(props, 1, &cdDevices[uiDeviceUsed], NULL, NULL, &err);
		try{
			context = cl::Context(CL_DEVICE_TYPE_GPU, props);
		}
		catch (cl::Error er) {
			printf("ERROR: %s(%s)\n", er.what(), oclErrorString(er.err()));
		}
#endif
#endif

		devices = context.getInfo<CL_CONTEXT_DEVICES>();

		queue = cl::CommandQueue(context, devices[0], 0);
	} catch (cl::Error error) {
		std::cerr << "caught exception: " << error.what() 
			<< '(' << error.err() << ')' << std::endl;
	}
}

OCLProg::OCLProg(const std::string& kernelFile)    
{

	initCL();

	std::ifstream file(kernelFile);
	std::string prog(std::istreambuf_iterator<char>(file),
		(std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(prog.c_str(), prog.length()+1));
	cl::Program program(context, source);
	file.close();

	try
	{
		program.build(devices);
	}
	catch(cl::Error& err)
	{
		std::cerr << "Build failed! " << err.what() 
			<< '(' << err.err() << ')' << std::endl;
		std::cerr << "retrieving  log ... " << std::endl;
		std::cerr 
			<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])
			<< std::endl;
		exit(-1);
	}
	cl_int err;
	try {

		simulateKernel = cl::Kernel(program, "simulate");
		generateKernel = cl::Kernel(program, "generate");

		posData = new cl_float4[vecLen]; // 4th index is TTL
		for(int i = 0; i < vecLen; ++i)
			for(int j = 0; j < 4; ++j)
				posData[i].s[j] = 0;
		glGenVertexArrays(1, &posVAOid);
		glBindVertexArray(posVAOid);

		glGenBuffers(1, &posVBOid);
		glBindBuffer(GL_ARRAY_BUFFER, posVBOid);
		glBufferData(GL_ARRAY_BUFFER, vecSize, posData, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glFinish();

		velBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, vecSize);
		accBuffer = cl::Buffer(context, CL_MEM_READ_ONLY, vecSize);
		posBuffer = cl::BufferGL(context, CL_MEM_READ_WRITE, posVBOid, &err);

		simulateKernel.setArg(3, accBuffer);
		simulateKernel.setArg(2, velBuffer);
		simulateKernel.setArg(1, posBuffer);

		generateKernel.setArg(0, posBuffer);
		cl_vbos.push_back(posBuffer);

	} catch (cl::Error error) {
		std::cerr << "caught exception: " << error.what() 
			<< '(' << error.err() << ')' << std::endl;
	}
	accelerations = new float[vecLen*4];
	velocities = new float[vecLen*4];
	srand(time(0));
	for(int i = 0; i < vecLen; ++i)
	{
		accelerations[i] = ((rand()%5000-2500))/100.0f;
		velocities[i] = ((rand()%5000-2500))/100.0f;
	}
}

void OCLProg::generate()
{

	cl::Event clevent;
	queue.enqueueWriteBuffer(accBuffer, CL_TRUE, 0, vecSize, accelerations, NULL, &clevent);
	queue.enqueueWriteBuffer(velBuffer, CL_TRUE, 0, vecSize, accelerations, NULL, &clevent);
	glFinish();
	queue.enqueueAcquireGLObjects(&cl_vbos, NULL, &clevent);
	queue.enqueueNDRangeKernel(generateKernel, cl::NullRange, cl::NDRange(vecLen), cl::NullRange, NULL, &clevent);
	queue.enqueueReleaseGLObjects(&cl_vbos, NULL, &clevent);
	queue.finish();
}

void OCLProg::simulate(float dt)
{
	glFinish();
	cl::Event clevent;
	simulateKernel.setArg(0, dt);
	queue.enqueueAcquireGLObjects(&cl_vbos, NULL, &clevent);
	queue.enqueueNDRangeKernel(simulateKernel, cl::NullRange, cl::NDRange(vecLen), cl::NullRange, NULL, &clevent);
	queue.enqueueReleaseGLObjects(&cl_vbos, NULL, &clevent);
	queue.finish();
}

void OCLProg::getData()
{
	cl::Event clevent;
	queue.enqueueAcquireGLObjects(&cl_vbos, NULL, &clevent);
	queue.enqueueReadBuffer(posBuffer, CL_TRUE, 0, vecSize, posData, NULL, &clevent);
	queue.enqueueReleaseGLObjects(&cl_vbos, NULL, &clevent);
	queue.finish();
}

void OCLProg::printData()
{
	for(int i = 0; i < vecLen; ++i)
	{
		for(int j = 0; j < 4; ++j)
			std::cout << posData[i].s[j] << " ";
		std::cout << '\n';
	}

}