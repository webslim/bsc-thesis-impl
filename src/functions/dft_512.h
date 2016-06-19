#ifndef DFT_512_H
#define DFT_512_H

#define FP_TYPE double

#include <cmath>
#ifdef CLASS
   #include "../array_class.h"
#endif
//#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "dft_512.cpp"

#endif

