#ifndef REC_MERGESORT_H
#define REC_MERGESORT_H

#define FP_TYPE double

#ifdef CLASS
#include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../array_class.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "rec_mergesort.cpp"

#endif

