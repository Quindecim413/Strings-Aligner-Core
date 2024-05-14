#include "ConvolveNMatches.h"

int convolveNMatches(PCQ* prog, int32_t* arr_short, size_t size_arr_short,
    int32_t* arr_long, size_t size_arr_long,
    int32_t* scores, size_t size_scores) {
    cl_int err = 0;
    cl::Buffer short_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS,
        sizeof(int) * size_arr_short, arr_short, &err);
    if (err) return err;
    cl::Buffer long_g(*prog->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS,
        sizeof(int) * size_arr_long, arr_long, &err);
    if (err) return err - 1000;
    cl::Buffer scores_g(*prog->context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * size_scores, nullptr, &err);
    if (err) return err - 2000;

    cl::Kernel kernel(*prog->program, "convolveNMatches", &err);
    if (err) {
        return err - 3000;
    }
    kernel.setArg(0, short_g);
    kernel.setArg(1, (int)size_arr_short);
    kernel.setArg(2, long_g);
    kernel.setArg(3, scores_g);
    size_t step = 1000000;
    for (size_t i = 0; i < size_scores; i += step) {
        int n_elements = std::min(size_scores - i, step);

        err = prog->queue->enqueueNDRangeKernel(kernel, cl::NDRange(i), cl::NDRange(n_elements));

        if (err) return err - 10000;
    }

    err = prog->queue->enqueueReadBuffer(scores_g, CL_TRUE,
        0, sizeof(int) * size_scores, scores);

    cl::finish();
    if (err) return err - 4000;
}