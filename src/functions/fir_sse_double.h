#ifndef FIR_SSE_DOUBLE_H
#define FIR_SSE_DOUBLE_H

#define FP_TYPE double

#include <emmintrin.h>
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "fir_sse_double.cpp"

#endif

