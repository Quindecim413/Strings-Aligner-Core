#pragma once
#define CL_USE_DEPRECETED_OPENCL_2_0_APIS
#include <CL/cl.hpp>
#include <fstream>
#include "PCQ.h"
#include <stdint.h>

extern "C" __declspec(dllexport)
void NWScoreCPU(int32_t* a, size_t size_a, int32_t * b, size_t size_b,
	int32_t * S, int32_t S_width, int32_t d, int32_t * scores);

extern "C" __declspec(dllexport)
int NWScoreGPU(PCQ* prog, 
	int32_t * a, size_t size_a, int32_t * b, size_t size_b,
	int32_t * S, int32_t S_width, int32_t d, int32_t * scores);

extern "C" __declspec(dllexport)
int NWAlignCPU(int32_t ident, int32_t * a, size_t size_a, int32_t * b, size_t size_b,
	int32_t * S, int32_t S_width, int32_t d,
	int32_t * a_aligned_buf, int32_t * b_aligned_buf);

extern "C" __declspec(dllexport)
int NWAlignGPU(PCQ* prog,
	int32_t * a, size_t size_a, int32_t * b, size_t size_b,
	int32_t * S, int32_t S_width, int32_t d,
	int32_t * a_aligned_buf, int32_t * b_aligned_buf);
