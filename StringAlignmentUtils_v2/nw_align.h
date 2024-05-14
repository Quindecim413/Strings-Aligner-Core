#pragma once
//#include "nw_utils.h"
//#include "PCQ.h"
//
//int NWAlignGPU(int threadIdent, PCQ* prog,
//	size_t a_start, size_t a_end,
//	size_t b_start, size_t b_end,
//	int32_t* a_aligned_buf, int32_t* b_aligned_buf, cl_int &err) {
//	int size_a = a_end - a_start;
//	int size_b = b_end - b_start;
//
//	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);
//	std::vector<std::pair<int, int>> starts = startsPerDiagonalLine(size_a, size_b);
//
//	int num_diagonals = starts.size();
//
//	cl_int err = 0;
//
//	int F_h = size_a + 1;
//	int F_w = size_b + 1;
//
//	auto dataGroup = prog->cachedData->createDataGroupIfNotExists(prog, threadIdent, err);
//	if (err) return 0;
//
//	// fill F
//	//dataGroup->prepareFplain()
//	//
//
//	//
//
//
//	////Timer<std::chrono::milliseconds, std::chrono::steady_clock> timer;
//	////timer.tick();
//
//	//for (int i_d = 0; i_d < num_diagonals; i_d++)
//	//{
//	//	int n_elements = num_elements[i_d];
//	//	std::pair<int, int> start = starts[i_d];
//	//	err = kernel.setArg(6, start.first);
//	//	if (err) {
//	//		return err - 11000;
//	//	}
//	//	err = kernel.setArg(7, start.second);
//	//	if (err) {
//	//		return err - 12000;
//	//	}
//	//	err = prog->queue->enqueueNDRangeKernel(
//	//		kernel,
//	//		cl::NullRange,
//	//		cl::NDRange(n_elements)
//	//	);
//
//	//	if (err) {
//	//		return err - 20000;
//	//	}
//	//}
//
//	////timer.tock();
//	////std::cout << timer.duration().count() << std::endl;
//
//	//err = prog->queue->enqueueReadBuffer(F_g, CL_TRUE,
//	//	0, sizeof(int) * F_h * F_w, F_plain);
//
//	//if (err) {
//	//	return err - 13000;
//	//}
//
//	//int ind = alignCodes(F_plain, a, size_a, b, size_b, S, S_width, d, a_aligned_buf, b_aligned_buf);
//	//delete[] F_plain;
//	//return ind;
//}
