#pragma once

#define CL_USE_DEPRECETED_OPENCL_2_0_APIS
#include <CL/cl.hpp>
#include <fstream>
#include <mutex>

class PCQ
{
public:
	cl::Program *program;
	cl::Context *context;
	cl::CommandQueue *queue;
	cl::STRING_CLASS selected_plaform_name;
	std::mutex m_lock;
	PCQ(cl::Platform platform, cl::Program::Sources sources);
	~PCQ();
	int build();
};

extern "C" __declspec(dllexport)
PCQ* createPCQ(int platform_ind);

extern "C" __declspec(dllexport)
int buildPCQ(PCQ* pcq);

extern "C" __declspec(dllexport)
void deletePCQ(PCQ* pcq); 

extern "C" __declspec(dllexport)
char* getPlatformNames();

extern "C" __declspec(dllexport)
void freeChars(char* arr);