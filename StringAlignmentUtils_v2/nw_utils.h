#pragma once
#include <vector>
#include <iostream>

void printArr(int32_t* arr, size_t size);
std::vector<int> numElementsPerDiagonalLine(size_t size_a, size_t size_b);
std::vector<std::pair<int, int>> startsPerDiagonalLine(size_t size_a, size_t size_b);
int alignCodes(int32_t* F_plain,
	int32_t* a, size_t size_a, int32_t* b, size_t size_b,
	int32_t* S, int32_t S_width, int32_t d,
	int32_t* a_aligned_buf, int32_t* b_aligned_buf);