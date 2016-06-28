#ifndef RFFTFWD_SSE_128
#define RFFTFWD_SSE_128

#define FP_TYPE double

#include <cmath>
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "rfftfwd_sse_128.cpp"

#endif

