#ifndef QUICKSORT_H
#define QUICKSORT_H

#define FP_TYPE double

#include <cstring>
#include "../copy_functions.h"
#include "../iterators.h"
#ifdef CLASS
#include "../array_class.h"
#endif
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *copy, double *out);
#include "quicksort.cpp"

#endif
