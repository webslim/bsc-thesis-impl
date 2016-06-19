#ifndef REDUCE_H
#define REDUCE_H

#define FP_TYPE double

#include <forward_list>
#ifdef CLASS
   #include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
//#include "../iterators_noref.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "reduce.cpp"

#endif

