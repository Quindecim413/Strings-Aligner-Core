#include "PCQ.h"
#include <iostream>


PCQ::PCQ(cl::Platform platform, cl::Program::Sources sources) {

	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	selected_plaform_name = platform.getInfo<CL_PLATFORM_NAME>();
	context = new cl::Context(devices);
	program = new cl::Program(*this->context, sources);
	//cl_int err = 0;
	queue = new cl::CommandQueue(*this->context);
	//std::cout << "CommandQueue " << err << std::endl;

}

PCQ::~PCQ() {
	delete this->queue;
	delete this->context;
	delete this->program;
}


int PCQ::build() {
	std::cout << "PCQ build" << std::endl;
	return program->build("-cl-std=CL1.2");
}


//extern "C" __declspec(dllexport)
PCQ* createPCQ(int platform_ind) {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	if (platform_ind >= platforms.size()) {
		return NULL;
	}

	/*std::ifstream code_file("code.cl");
	std::string src(std::istreambuf_iterator<char>(code_file), (std::istreambuf_iterator<char>()));*/

	std::string src(
		"__kernel void makeNW("
		"__global const int* a_g, __global const int* b_g, __global int* F,"
		"__constant int* s, int alphabet_size,"
		"int d, int start_i, int start_j, int size_width){"
		"int gid = get_global_id(0);"
		"int i = start_i + gid;"
		"int j = start_j - gid;"
		"int similarity = s[a_g[i - 1] * alphabet_size + b_g[j - 1]];"
		""
		"int _match = F[(j - 1) + size_width * (i - 1)] + similarity;"
		"int _delete = F[j + size_width * (i - 1)] + d;"
		"int _insert = F[(j - 1) + size_width * i] + d;"
		"int _max = _match > _delete ? _match : _delete;"
		"_max = _max > _insert ? _max : _insert;"
		"F[j + size_width * i] = _max;"
		"}"
		"__kernel void makeNWScore("
		"__global const int* a_g, __global const int* b_g,"
		"__global int* prev, __global int* curr, __global int* next,"
		"__constant int* s, int alphabet_size,"
		"__global int* res,"
		"int len_a, int len_b,"
		"int d, int start, int iter_num){"
		"int gid = get_global_id(0);"
		"int pos = start + gid;"
		"int _a = a_g[iter_num + 1 - pos];"
		"int _b = b_g[pos - 1];"
		"int similarity = s[_a * alphabet_size + _b];"
		""
		"int _match = prev[pos - 1] + similarity;"
		"int _delete = curr[pos] + d;"
		"int _insert = curr[pos - 1] + d;"
		""
		"int _max = _match > _delete ? _match : _delete;"
		"_max = _max > _insert ? _max : _insert;"
		""
		"next[pos] = _max;"
		"if (gid == 0) {"
		"	if (iter_num < len_a - 1) {"
		"		next[0] = (iter_num + 2) * d;"
		"	}"
		"	if (iter_num < len_b - 1) {"
		"		next[iter_num + 2] = (iter_num + 2) * d;"
		"	}"
		"	if (iter_num + 1 >= len_a) {"
		"		res[iter_num + 2 - len_a] = next[pos];"
		"	}"
		"}"
		"}"
		"__kernel void convolveNMatches(__global const int* short_arr, int size_short_arr,"
		"	__global const int* long_arr,"
		"	__global int* scores) {"
		"int i = get_global_id(0);"
		"int score = 0;"
		"for (int j = 0; j < size_short_arr; j++) {"
		"	score += short_arr[j] == long_arr[i + j];"
		"}"
		"scores[i] = score;"
		"}"
	);
	//std::cout << "FILE SOURCES" << src;
	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	auto platform = platforms[platform_ind];
	printf("creating PCQ\r\n");
	return new PCQ(platform, sources);
}

//PCQ* createPCQWithCachedData(int32_t platform_ind, int* shortArr, size_t shortArrSize, int* longArr, size_t longArrSize,
//	int* SPlain, size_t SWidth, int32_t d, int32_t NWAlingMaxSequenceSize) {
//	return nullptr;
//}

//extern "C" __declspec(dllexport)
int buildPCQ(PCQ* pcq) {
	printf("building PCQ\r\n");
	return pcq->build();
}

//extern "C" __declspec(dllexport)
void deletePCQ(PCQ* pcq) {
	printf("deleting PCQ\r\n");
	delete pcq;
}

char* getPlatformNames() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	std::string res_string("");

	for (auto const& platform : platforms) {
		res_string = res_string.append(platform.getInfo<CL_PLATFORM_NAME>()) + "\n";
	}

	char* res = new char[res_string.size() - 1];
	res_string.copy(res, res_string.size() - 1, 0);
	res[res_string.size() - 1] = '\0';

	return res;
}

void freeChars(char* arr) {
	delete[] arr;
}