#ifndef SPIRAL_RFFTFWD_DOUBLE_128
#define SPIRAL_RFFTFWD_DOUBLE_128

#define FP_TYPE double

#include <cmath>
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "spiral_rfftfwd_double_128.cpp"

#endif

