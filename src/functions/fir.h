#ifndef FIR_H
#define FIR_H

#define FP_TYPE double

#ifdef CLASS
   #include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "fir.cpp"

#endif

