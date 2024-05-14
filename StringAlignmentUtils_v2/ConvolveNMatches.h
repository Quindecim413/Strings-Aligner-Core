#pragma once
#include "PCQ.h"
#include <CL/cl.hpp>

extern "C" __declspec(dllexport)
int convolveNMatches(PCQ * pcq, int32_t * arr_short, size_t size_arr_short,
    int32_t * arr_long, size_t size_arr_long,
    int32_t * scores, size_t size_scores);


