#ifndef DFT_1048576_H
#define DFT_1048576_H

#define FP_TYPE double

#include <cmath>
#ifdef CLASS
#include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "dft_1048576.cpp"

#endif

