#include "DataGroup.h"

std::shared_ptr<DataGroup> DataGroup::create(PCQ* pcq, int NWAlingMaxSequenceSize, int NWScoreMaxSequenceSize, cl_int& err) {
	std::shared_ptr<DataGroup> d = std::make_shared<DataGroup>();

	d->FPlain_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY,
		sizeof(int) * (NWAlingMaxSequenceSize + 1) * (NWAlingMaxSequenceSize + 1),
		nullptr, &err);
	if (err) {
		err += 1000;
		return d;
	};
	d->prev_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * (NWScoreMaxSequenceSize + 1),
		nullptr, &err);
	if (err) {
		err += 2000;
		return d;
	};
	d->curr_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * (NWScoreMaxSequenceSize + 1),
		nullptr, &err);
	if (err) {
		err += 3000;
		return d;
	};
	d->next_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * (NWScoreMaxSequenceSize + 1),
		nullptr, &err);
	if (err) {
		err += 4000;
		return d;
	};

	d->res = new int[NWScoreMaxSequenceSize + 1];
	d->res_inv = new int[NWScoreMaxSequenceSize + 1];
	d->FPlain = new int[(NWScoreMaxSequenceSize + 1) * NWScoreMaxSequenceSize + 1];
	return d;
}

DataGroup::~DataGroup() {
	delete NWAlignKernel, FillFVertKernel, FillFHorKernel, NWScoreKernel, NWScoreInvKernel, SetCurrValuesKernel, SetCurrValuesInvKernel;
	delete[] res, res_inv, FPlain;
}

void DataGroup::prepareFplain(PCQ* pcq, int size_a, int size_b, int d, cl_int& err) {

}

void DataGroup::prepareCurr(PCQ* pcq, int d, cl_int& err) {

}

void DataGroup::prepareCurrInv(PCQ* pcq, int d, cl_int& err) {

}