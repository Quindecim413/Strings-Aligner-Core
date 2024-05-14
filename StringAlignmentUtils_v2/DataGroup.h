#pragma once
#include <CL/cl.hpp>
#include "PCQ.h"
#include <mutex>

class DataGroup {
public:
	cl::Buffer FPlain_g;
	int* FPlain;
	cl::Buffer prev_g;
	cl::Buffer curr_g;
	cl::Buffer next_g;

	cl::Buffer res_g;
	int* res;
	int* res_inv;


	cl::Kernel* NWAlignKernel;
	cl::Kernel* FillFVertKernel;
	cl::Kernel* FillFHorKernel;

	void prepareFplain(PCQ* pcq, int size_a, int size_b, int d, cl_int & err);
	void prepareCurr(PCQ* pcq, int d, cl_int& err);
	void prepareCurrInv(PCQ* pcq, int d, cl_int& err);

	cl::Kernel* NWScoreKernel;
	cl::Kernel* SetCurrValuesKernel;
	cl::Kernel* NWScoreInvKernel;
	cl::Kernel* SetCurrValuesInvKernel;

	static std::shared_ptr<DataGroup> create(PCQ* pcq, int NWAlingMaxSequenceSize, int NWScoreMaxSequenceSize, cl_int& err);
	~DataGroup();
};

