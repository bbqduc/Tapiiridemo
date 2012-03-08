#pragma once

#include <GL3/gl3w.h>

#define __CL_ENABLE_EXCEPTIONS
#define __NO_STD_VECTOR

#include <CL/cl.hpp>
#include <fstream>
#include <string>
#include <iostream>

class OCLProg
{
	cl::Context context;
	cl::vector<cl::Device> devices;
	cl::vector<cl::Memory> cl_vbos;
	cl::CommandQueue queue;
	cl::Kernel generateKernel, simulateKernel;
	cl::BufferGL posBuffer;
	cl::Buffer velBuffer, accBuffer;

	float *accelerations, *velocities;

	void initCL();
public:
	static const unsigned int vecLen = 65536/2;
	static const unsigned int vecSize = vecLen*sizeof(cl_float4);

	cl_float4* posData;

	GLuint posVBOid, posVAOid;

	OCLProg(const std::string&);
	void generate();
	void simulate(float dt);
	void getData();
	void printData();
};
