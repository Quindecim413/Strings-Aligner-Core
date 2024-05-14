#pragma once
#include <CL/cl.hpp>
#include <map>
#include "PCQ.h"
#include <mutex>
#include "DataGroup.h"


class CachedData
{
public:
	cl::Buffer S;
	int SWidth;
	int NWAlingMaxSequenceSize;
	int NWScoreMaxSequenceSize;
	cl::Buffer shortArr_g;
	cl::Buffer longArr_g;
	std::vector<int> shortArr;
	std::vector<int> longArr;
	int d;

	std::shared_ptr<DataGroup> createDataGroupIfNotExists(PCQ* pcq, int threadIdent, cl_int &err);
	bool deleteDataGroupIfExists(int threadIdent);
	static CachedData* create(PCQ* pcq, int NWAlingMaxSequenceSize, std::vector<int> shortArr, std::vector<int> longArr, std::vector<std::vector<int>> S, int d, cl_int &err);
private:
	std::map<int, std::shared_ptr<DataGroup>> cachedPerThreadDataGroups;
	std::mutex changeMapMutex;
};

