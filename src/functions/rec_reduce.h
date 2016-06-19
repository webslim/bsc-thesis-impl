#ifndef REC_REDUCE_H
#define REC_REDUCE_H

#define FP_TYPE double

#include <forward_list>
#ifdef CLASS
   #include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "rec_reduce.cpp"

#endif

