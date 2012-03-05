#include "oclfile.h"
#include <utility>
#include <cstdlib>
#include <ctime>

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
		std::cout << "Context : " << wglGetCurrentContext() << "\n";
		std::cout << "DC : " << wglGetCurrentDC() << "\n";

		cl_context_properties cprops[] = 
		{CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), 
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(),
		0};

		context = cl::Context(CL_DEVICE_TYPE_GPU, cprops);

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
		if(err == CL_OUT_OF_HOST_MEMORY)
			std::cerr << "HERP" << std::endl;
		std::cerr << "caught exception: " << error.what() 
			<< '(' << error.err() << ')' << std::endl;
	}
	accelerations = new float[vecLen*4];
	velocities = new float[vecLen*4];
	srand(time(0));
	for(int i = 0; i < vecLen; ++i)
	{
		accelerations[i] = (rand()%50-25);
		velocities[i] = (rand()%50-25);
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