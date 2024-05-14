#include "NWComputers.h"
#include <iostream>

#define MAX 10

void printArr(int32_t* arr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		std::cout << arr[i] << "\t";
	}
	std::cout << "\n";
}

std::vector<int> numElementsPerDiagonalLine(size_t size_a, size_t size_b) {
	int n_rows = size_a + 1;
	int n_cols = size_b + 1;
	int min_of_dims = std::min(n_rows, n_cols);
	int diff_of_dims = std::abs(n_rows - n_cols);

	int total_elements = min_of_dims * 2 - 2;
	int repeats = 0;

	if (diff_of_dims == 0) {
		total_elements--;
	}
	if (diff_of_dims - 1 > 0) {
		repeats = diff_of_dims - 1;
		total_elements += repeats;
	}

	std::vector<int> nums_elements(total_elements);
	for (int i = 0; i < min_of_dims - 1; i++) {
		nums_elements[i] = i + 1;
	}
	if (repeats > 0) {
		for (int i = min_of_dims - 1; i < min_of_dims - 1 + repeats; i++) {
			nums_elements[i] = min_of_dims - 1;
		}
	}

	for (int i = 0; i < min_of_dims - 1; i++) {
		nums_elements[total_elements - i - 1] = i + 1;
	}
	return nums_elements;
}

std::vector<std::pair<int, int>> startsPerDiagonalLine(size_t size_a, size_t size_b) {
	int n_rows = size_a + 1;
	int n_cols = size_b + 1;

	std::vector<std::pair<int, int>> starts(n_rows + n_cols - 3);

	for (int j = 0; j < n_cols - 1; j++) {
		starts[j] = std::pair<int, int>(1, j + 1);
	}
	for (int i = 2; i < n_rows; i++) {
		starts[(i - 2) + n_cols - 1] = std::pair<int, int>(i, n_cols-1);
	}
	return starts;
}


int alignCodes(int32_t* F_plain,
	int32_t* a, size_t size_a, int32_t* b, size_t size_b,
	int32_t* S, int32_t S_width, int32_t d,
	int32_t* a_aligned_buf, int32_t* b_aligned_buf) {

	/*std::cout << "F" << std::endl;
	for (int i = 0; i < size_a + 1; i++) {
		printArr(F_plain + (size_b * i), size_b);
	}*/

	int score = 0;
	int diag = 0;
	int up = 0;
	int left = 0;
	size_t i = size_a, j = size_b;
	int _a = 0, _b = 0, ind = 0;

	//std::cout << "alignCodes" << std::endl;

	int F_h = size_a + 1;
	int F_w = size_b + 1;

	while ((i > 0) && (j > 0)) {
		score = F_plain[i* F_w + j];
		diag = F_plain[(i - 1) * F_w + (j - 1)];
		left = F_plain[i * F_w + (j - 1)];
		up = F_plain[(i - 1) * F_w + j];
		//std::cout << "i,j = (" << i << ", " << j << ')' << std::endl;
		//std::cout << "[[" << diag << ", " << up << "], " << '[' << left << ", " << score << "]]" << std::endl;
		_a = a[i - 1];
		_b = b[j - 1];
		if (score == left + d) {
			a_aligned_buf[ind] = -1;
			b_aligned_buf[ind] = _b;
			j--;
		}
		else if (score == up + d) {
			a_aligned_buf[ind] = _a;
			b_aligned_buf[ind] = -1;
			i--;
		} else {
			a_aligned_buf[ind] = _a;
			b_aligned_buf[ind] = _b;
			i--; j--;
		}
		/*if (score == left + d) {
			a_aligned_buf[ind] = -1;
			b_aligned_buf[ind] = _b;
			j--;
		}else
		if (score == diag + S[_a*S_width + _b]) {
			a_aligned_buf[ind] = _a;
			b_aligned_buf[ind] = _b;
			i--; j--;
		} else {
			a_aligned_buf[ind] = _a;
			b_aligned_buf[ind] = -1;
			i--;
		}*/
		ind++;
	}

	//std::cout << "while (i > 0)" << std::endl;
	while (i > 0) {
		//std::cout << "i,j = (" << i << ", " << j << ')' << std::endl;
		_a = a[i - 1];
		a_aligned_buf[ind] = _a;
		b_aligned_buf[ind] = -1;
		i--; ind++;
	}
	//std::cout << "while (j > 0)" << std::endl;
	while (j > 0) {
		//std::cout << "i,j = (" << i << ", " << j << ')' << std::endl;
		_b = b[j - 1];
		a_aligned_buf[ind] = -1;
		b_aligned_buf[ind] = _b;
		j--;
		ind += 1;
	}
	return ind;
}

void NWScoreCPU(int32_t* a, size_t size_a, int32_t* b, size_t size_b, int32_t* S, int32_t S_width, int32_t d, int32_t* scores) {
	//std::cout << "NWScoreCPU" <<std::endl;
	int32_t* prev = new int32_t[size_b + 1];
	int32_t* curr = new int32_t[size_b + 1];
	int32_t* next = new int32_t[size_b + 1];

	prev[0] = 0;

	scores[0] = size_a * d;
	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);
	int start = 1;
	int n_el = 0;

	size_t n_els = num_elements.size();
	for (size_t iter = 0; iter < n_els; iter++) {
		if (iter < size_a) {
			start = 1;
			curr[0] = (iter + 1) * d;
		}
		else {
			start++;
		}


		if (iter < size_b) {
			curr[iter + 1] = (iter + 1) * d;
		}
		
		n_el = num_elements[iter];

		//#pragma omp parallel num_threads(4)
		for (size_t pos = start; pos < start + n_el; pos++) {
			auto p = prev[pos - 1];
			//auto l = 
			int s = p + S[a[iter + 1 - pos] * S_width + b[pos - 1]];
			next[pos] = std::max(s,
							std::max(
								curr[pos] + d,
								curr[pos - 1] + d
							));
		}

		if (iter + 1 >= size_a) {
			scores[iter + 2 - size_a] = next[start];
		}

		/*std::cout << "start = " << start << " n_el = " << n_el << " iter = " << iter << std::endl;
		std::cout << "prev" << std::endl;
		printArr(prev, size_b + 1);

		std::cout << "curr" << std::endl;
		printArr(curr, size_b + 1);

		std::cout << "next " << std::endl;
		printArr(next, size_b + 1);

		std::cout << "res " << std::endl;
		printArr(scores, size_b + 1);

		std::cout << std::endl;*/

		int* tmp = prev;
		prev = curr;
		curr = next;
		next = tmp;
	}
	delete[] prev, curr, next;
}

int NWScoreGPU(PCQ* prog, int32_t* a, size_t size_a, int32_t* b, size_t size_b, int32_t* S, int32_t S_width, int32_t d, int32_t* scores) {
	cl_int err = 0;
	cl::Buffer a_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_a, a, &err);
	if (err) {
		return err;
	}
	cl::Buffer b_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_b, b, &err);
	if (err) {
		return err - 1000;
	}
	cl::Buffer s_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * S_width * S_width, S, &err);
	if (err) {
		return err - 2000;
	}
	cl::Buffer prev_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS, sizeof(int) * (size_b + 1), nullptr, &err);
	if (err) {
		return err - 3000;
	}
	cl::Buffer curr_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS, sizeof(int) * (size_b + 1), nullptr, &err);
	if (err) {
		return err - 4000;
	}
	cl::Buffer next_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS, sizeof(int) * (size_b + 1), nullptr, &err);
	if (err) {
		return err - 5000;
	}

	//cl::Buffer prev_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(int) * (size_b + 1), nullptr, &err);
	//if (err) {
	//	return err - 3000;
	//}
	//cl::Buffer curr_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(int) * (size_b + 1), nullptr, &err);
	//if (err) {
	//	return err - 4000;
	//}
	//cl::Buffer next_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, sizeof(int) * (size_b + 1), nullptr, &err);
	//if (err) {
	//	return err - 5000;
	//}

	prog->queue->enqueueFillBuffer(prev_g, 0, 0, sizeof(int) * 1);
	prog->queue->enqueueFillBuffer(curr_g, d, 0, sizeof(int) * 2);

	cl::Buffer scores_g(*prog->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * (size_b+1), nullptr, &err);

	cl::Kernel kernel(*prog->program, "makeNWScore");
	err = kernel.setArg(0, a_g);
	if (err) {
		return err - 6000;
	}
	err = kernel.setArg(1, b_g);
	if (err) {
		return err - 7000;
	}
	//err = kernel.setArg(2, *prev_g);
	//err = kernel.setArg(3, *curr_g);
	//err = kernel.setArg(4, *next_g);
	err = kernel.setArg(5, s_g);
	if (err) {
		return err - 8000;
	}
	err = kernel.setArg(6, S_width);
	if (err) {
		return err -9000;
	}
	err = kernel.setArg(7, scores_g);
	if (err) {
		return err - 10000;
	}
	err = kernel.setArg(8, (int)size_a);
	if (err) {
		return err - 11000;
	}
	err = kernel.setArg(9, (int)size_b);
	if (err) {
		return err - 12000;
	}
	err = kernel.setArg(10, d);
	if (err) {
		return err - 13000;
	}
	//err = kernel.setArg(11, start);
	//err = kernel.setArg(12, iter);

	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);
	int start = 1;
	int n_el = 0;

	cl::Buffer* tmp = nullptr;

	int* tmp_arr = new int[size_b + 1];

	int n_els = num_elements.size();

	int inds[] = {0, 1, 2};
	cl::Buffer buffers[] = {prev_g, curr_g, next_g};
	for (int iter = 0; iter < n_els; iter++) {
		if (iter >= size_a) {
			start++;
		}
		n_el = num_elements[iter];

		err = kernel.setArg(2, buffers[inds[0]]);
		if (err) {
			return err - 14000;
		}
		err = kernel.setArg(3, buffers[inds[1]]);
		if (err) {
			return err - 15000;
		}
		err = kernel.setArg(4, buffers[inds[2]]);
		if (err) {
			return err - 16000;
		}
		err = kernel.setArg(11, start);
		if (err) {
			return err - 17000;
		}
		err = kernel.setArg(12, iter);
		if (err) {
			return err - 18000;
		}

		/*std::cout << "start = " << start << " n_el = " << n_el << " iter = " << iter << std::endl;
		std::cout << "prev 00" << std::endl;
		err = prog->queue->enqueueReadBuffer(buffers[inds[0]], CL_TRUE,
			0, sizeof(int32_t) * (size_b + 1), tmp_arr);
		printArr(tmp_arr, size_b + 1);

		std::cout << "curr 00" << std::endl;
		err = prog->queue->enqueueReadBuffer(buffers[inds[1]], CL_TRUE,
			0, sizeof(int32_t) * (size_b + 1), tmp_arr);
		printArr(tmp_arr, size_b + 1);

		std::cout << "next 00" << std::endl;
		err = prog->queue->enqueueReadBuffer(buffers[inds[2]], CL_TRUE,
			0, sizeof(int32_t) * (size_b + 1), tmp_arr);
		printArr(tmp_arr, size_b + 1);*/

		err = prog->queue->enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(n_el)
		);
		if (err) {
			return err - 20000;
		}
		

		std::swap(inds[0], inds[2]);
		std::swap(inds[0], inds[1]);
	}

	err = prog->queue->enqueueReadBuffer(scores_g, CL_TRUE,
		0, sizeof(int32_t) * (size_b+1), scores);
	
	if (err) {
		return err - 19000;
	}
	scores[0] = size_a * d;

	delete[] tmp_arr;

	cl::finish();
}


int NWAlignCPU(int32_t ident, int32_t* a, size_t size_a, int32_t* b, size_t size_b,
	int32_t* S, int32_t S_width, int32_t d,
	int32_t* a_aligned_buf, int32_t* b_aligned_buf) {
	//std::cout << "ident => " << ident << std::endl;
	int F_h = size_a + 1;
	int F_w = size_b + 1;

	int* F_plain = new int[F_h * F_w];

	for (int j = 0; j < F_w; j++) {
		F_plain[j] = j * d;
	}

	for(int i = 0; i < F_h; i++){
		F_plain[i * F_w] = i * d;
	}

	//std::cout << "ident at " << ident << " BEFORE numElementsPerDiagonalLine\r\n";
	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);

	//std::cout << "ident at " << ident << " BEFORE startsPerDiagonalLine\r\n";
	std::vector<std::pair<int, int>> starts = startsPerDiagonalLine(size_a, size_b);

	int num_diagonals = starts.size();

	//Timer<std::chrono::milliseconds, std::chrono::steady_clock> timer;
	//timer.tick();

	for (int i_d = 0; i_d < num_diagonals; i_d++)
	{
		int n_elements = num_elements[i_d];
		std::pair<int, int> start = starts[i_d];
		//std::cout << i_d << '/' << num_diagonals << std::endl;
		
		for (int n_el = 0; n_el < n_elements; n_el++) {
			
			int i = start.first + n_el;
			int j = start.second - n_el;
			int _a = a[i - 1];
			int _b = b[j - 1];
			int s = S[_a * S_width + _b];
			/*std::cout << i << ',' << j << std::endl;*/
			F_plain[i * F_w + j] = std::max(F_plain[(i - 1) * F_w + j - 1] + s,
				std::max(F_plain[(i - 1) * F_w + j] + d,
					F_plain[i * F_w + j - 1] + d));
			
		}
	}

	//timer.tock();
	//std::cout << timer.duration().count() << std::endl;

	//std::cout << "ident at " << ident << " alinging\r\n";
	int ind = alignCodes(F_plain, a, size_a, b, size_b, S, S_width, d, a_aligned_buf, b_aligned_buf);
	//std::cout << "ident at " << ident << " alinging DONE\r\n";
	delete[] F_plain;
	return ind;
}

//int NWAlignGPU(int32_t ident, PCQ* prog,
//	int32_t* a, size_t size_a, int32_t* b, size_t size_b,
//	int32_t* S, int32_t S_width, int32_t d,
//	int32_t* a_aligned_buf, int32_t* b_aligned_buf,
//	int32_t* F_plain) {
//	std::vector<std::vector<int>> F(size_a, std::vector<int>(size_b));
//
//	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);
//	std::vector<std::pair<int, int>> starts = startsPerDiagonalLine(size_a, size_b);
//
//	int num_diagonals = starts.size();
//
//	cl_int err = 0;
//	cl::Buffer a_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_a, a, &err);
//	if (err) {
//		return err;
//	}
//	cl::Buffer b_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_b, b, &err);
//	if (err) {
//		return err - 1000;
//	}
//	cl::Buffer s_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * S_width * S_width, S, &err);
//	if (err) {
//		return err - 2000;
//	}
//
//	int F_h = size_a + 1;
//	int F_w = size_b + 1;
//	//int* F_plain = new int[F_h * F_w];
//	//std::cout << "ident " << ident << "BEFORE FILL F" << std::endl;
//	for (int i = 0; i < F_h * F_w; i++)
//		F_plain[i] = 0;
//
//	for (int j = 0; j < F_w; j++) {
//		F_plain[j] = j * d;
//	}
//
//	for (int i = 0; i < F_h; i++) {
//		F_plain[i * F_w] = i * d;
//	}
//
//
//	//std::cout << "ident " << ident << "BEFORE COPY F" << std::endl;
//	cl::Buffer F_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_READ_ONLY, sizeof(int) * F_h * F_w, F_plain, &err);
//
//	if (err) {
//		return err - 2500;
//	}
//
//	//std::cout << "ident " << ident << "BEFORE MAKE KERNEL" << std::endl;
//	cl::Kernel kernel(*prog->program, "makeNW", &err);
//	if (err) {
//		return err - 3000;
//	}
//
//	err = kernel.setArg(0, a_g);
//	if (err) {
//		return err - 4000;
//	}
//	err = kernel.setArg(1, b_g);
//	if (err) {
//		return err - 5000;
//	}
//	err = kernel.setArg(2, F_g);
//	if (err) {
//		return err - 6000;
//	}
//	err = kernel.setArg(3, s_g);
//	if (err) {
//		return err - 7000;
//	}
//	err = kernel.setArg(4, (int)S_width);
//	if (err) {
//		return err - 8000;
//	}
//	err = kernel.setArg(5, (int)d);
//	if (err > 0) {
//		return err - 9000;
//	}
//	err = kernel.setArg(8, (int)F_w);
//	if (err) {
//		return err - 10000;
//	}
//	err = prog->queue->enqueueReadBuffer(F_g, CL_TRUE,
//		0, sizeof(int) * F_h * F_w, F_plain);
//
//	//std::cout << "F initial " << F_w << ',' << F_h << std::endl;
//	/*for (int i = 0; i < F_h; i++) {
//		printArr(F_plain + i * F_w, F_w);
//	}*/
//
//	//std::cout << "ident " << ident << "BEFORE RUN" << std::endl;
//	for (int i_d = 0; i_d < num_diagonals; i_d++)
//	{
//		int n_elements = num_elements[i_d];
//		std::pair<int, int> start = starts[i_d];
//		err = kernel.setArg(6, start.first);
//		if (err) {
//			return err - 11000;
//		}
//		err = kernel.setArg(7, start.second);
//		if (err) {
//			return err - 12000;
//		}
//		//prog->m_lock.lock();
//		err = prog->queue->enqueueNDRangeKernel(
//			kernel,
//			cl::NullRange,
//			cl::NDRange(n_elements)
//		);
//		//prog->m_lock.unlock();
//		/*std::cout << i_d << '/' << num_diagonals << std::endl;*/
//		/*err = prog->queue->enqueueReadBuffer(F_g, CL_FALSE,
//			0, sizeof(int) * F_h * F_w, F_plain);
//		*/
//		//std::cout << "F " << i_d << " " << n_elements << " (" << start.first << "," << start.second << ")" << std::endl;
//		/*for (int i = 0; i < F_h; i++) {
//			printArr(F_plain + i * F_w, F_w);
//		}*/
//
//		if (err) {
//			return err - 20000;
//		}
//	}
//
//
//	//std::cout << "ident " << ident << "AFTER RUN" << std::endl;
//	err = prog->queue->enqueueReadBuffer(F_g, CL_TRUE,
//		0, sizeof(int) * F_h * F_w, F_plain);
//	//std::cout << "ident " << ident << "AFTER COPY F FROM HOST" << std::endl;
//
//	if (err) {
//		return err - 13000;
//	}
//
//	int ind = alignCodes(F_plain, a, size_a, b, size_b, S, S_width, d, a_aligned_buf, b_aligned_buf);
//	//delete[] F_plain;
//	return ind;
//}

int NWAlignGPU(PCQ* prog, 
	int32_t* a, size_t size_a, int32_t* b, size_t size_b,
	int32_t* S, int32_t S_width, int32_t d,
	int32_t* a_aligned_buf, int32_t* b_aligned_buf) {
	std::vector<std::vector<int>> F(size_a, std::vector<int>(size_b));

	auto num_elements = numElementsPerDiagonalLine(size_a, size_b);
	std::vector<std::pair<int, int>> starts = startsPerDiagonalLine(size_a, size_b);

	int num_diagonals = starts.size();

	cl_int err = 0;
	cl::Buffer a_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_a, a, &err);
	if (err) {
		return err;
	}
	cl::Buffer b_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * size_b, b, &err);
	if (err) {
		return err - 1000;
	}
	cl::Buffer s_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS, sizeof(int) * S_width * S_width, S, &err);
	if (err) {
		return err - 2000;
	}
	
	int F_h = size_a + 1;
	int F_w = size_b + 1;
	int* F_plain = new int[F_h * F_w];

	for (int i = 0; i < F_h * F_w; i++)
		F_plain[i] = 0;

	for (int j = 0; j < F_w; j++) {
		F_plain[j] = j * d;
	}

	for (int i = 0; i < F_h; i++) {
		F_plain[i * F_w] = i * d;
	}
	


	cl::Buffer F_g(*prog->context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_READ_ONLY, sizeof(int) * F_h * F_w, F_plain, &err);

	if (err) {
		return err - 2500;
	}

	cl::Kernel kernel(*prog->program, "makeNW", &err);
	if (err) {
		return err - 3000;
	}

	err = kernel.setArg(0, a_g);
	if (err) {
		return err - 4000;
	}
	err = kernel.setArg(1, b_g);
	if (err) {
		return err - 5000;
	}
	err = kernel.setArg(2, F_g);
	if (err) {
		return err - 6000;
	}
	err = kernel.setArg(3, s_g);
	if (err) {
		return err - 7000;
	}
	err = kernel.setArg(4, (int)S_width);
	if (err) {
		return err - 8000;
	}
	err = kernel.setArg(5, (int)d);
	if (err > 0) {
		return err - 9000;
	}
	err = kernel.setArg(8, (int)F_w);
	if (err) {
		return err - 10000;
	}
	err = prog->queue->enqueueReadBuffer(F_g, CL_TRUE,
		0, sizeof(int) * F_h * F_w, F_plain);

	//Timer<std::chrono::milliseconds, std::chrono::steady_clock> timer;
	//timer.tick();

	for (int i_d = 0; i_d < num_diagonals; i_d++)
	{
		int n_elements = num_elements[i_d];
		std::pair<int, int> start = starts[i_d];
		err = kernel.setArg(6, start.first);
		if (err) {
			return err - 11000;
		}
		err = kernel.setArg(7, start.second);
		if (err) {
			return err - 12000;
		}
		err = prog->queue->enqueueNDRangeKernel(
			kernel,
			cl::NullRange,
			cl::NDRange(n_elements)
		);

		if (err) {
			return err - 20000;
		}
	}

	//timer.tock();
	//std::cout << timer.duration().count() << std::endl;

	err = prog->queue->enqueueReadBuffer(F_g, CL_TRUE,
		0, sizeof(int) * F_h * F_w, F_plain);

	if (err) {
		return err - 13000;
	}

	int ind = alignCodes(F_plain, a, size_a, b, size_b, S, S_width, d, a_aligned_buf, b_aligned_buf);
	delete[] F_plain;
	return ind;
}