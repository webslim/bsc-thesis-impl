#ifndef SSE_MVM_H
#define SSE_MVM_H

#define FP_TYPE double

#include <cmath>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include "../alignment.h"
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "sse_mvm.cpp"

#endif
