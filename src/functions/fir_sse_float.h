#ifndef FIR_SSE_FLOAT_H
#define FIR_SSE_FLOAT_H

#define FP_TYPE float

#include <emmintrin.h>
#include "../alignment.h"
#ifdef CLASS
#include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(float *in, float *copy, float *out);
#include "fir_sse_float.cpp"

#endif

