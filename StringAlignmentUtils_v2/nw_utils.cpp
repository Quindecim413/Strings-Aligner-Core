#include "nw_utils.h"


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
		starts[(i - 2) + n_cols - 1] = std::pair<int, int>(i, n_cols - 1);
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
		score = F_plain[i * F_w + j];
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
		}
		else {
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