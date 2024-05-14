#include "CachedData.h"

CachedData* CachedData::create(PCQ* pcq, int NWAlingMaxSequenceSize, std::vector<int> shortArr, std::vector<int> longArr, std::vector<std::vector<int>> S, int d, cl_int& err) {
	CachedData* cachedData = new CachedData();
	cachedData->shortArr = shortArr;
	cachedData->longArr = longArr;
	cachedData->d = d;
	cachedData->SWidth = S.size();
	cachedData->NWAlingMaxSequenceSize = NWAlingMaxSequenceSize;
	cachedData->NWScoreMaxSequenceSize = shortArr.size();

	std::vector<int> s_plain;
	for (int i = 0; i < S.size(); i++) {
		s_plain.insert(s_plain.end(), S[i].begin(), S[i].end());
	}
	cachedData->S = cl::Buffer(*pcq->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * S.size() * S.size(), s_plain.data(), &err);
	if (err) {
		err += 10000;
		return cachedData;
	}
	cachedData->shortArr_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * shortArr.size() , shortArr.data(), &err);
	if (err) {
		err += 11000;
		return cachedData;
	}
	cachedData->longArr_g = cl::Buffer(*pcq->context,
		CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS,
		sizeof(int) * longArr.size(), longArr.data(), &err);
	if (err) {
		err += 12000;
		return cachedData;
	}

	return cachedData;
}

std::shared_ptr<DataGroup> CachedData::createDataGroupIfNotExists(PCQ* pcq, int threadIdent, cl_int& err) {
	std::unique_lock<std::mutex> lock(changeMapMutex);
	if (!cachedPerThreadDataGroups.count(threadIdent)) {
		auto dataGroup = DataGroup::create(pcq, NWAlingMaxSequenceSize, NWScoreMaxSequenceSize, err);
		if (err) {
			err += 13000;
			return dataGroup;
		}
		cachedPerThreadDataGroups.insert(std::make_pair(threadIdent, dataGroup));
		return dataGroup;
	}
	return cachedPerThreadDataGroups[threadIdent];
}

bool CachedData::deleteDataGroupIfExists(int threadIdent) {
	std::unique_lock<std::mutex> lock(changeMapMutex); 
	if (cachedPerThreadDataGroups.count(threadIdent)) {
		auto it = cachedPerThreadDataGroups.find(threadIdent);
		cachedPerThreadDataGroups.erase(it);
		return true;
	}
	return false;
}
