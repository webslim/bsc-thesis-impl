#ifndef FILTER_H
#define FILTER_H

#define FP_TYPE double

#include <cmath>
#ifdef CLASS
#include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"
#include "../randomize.h"

static void test(double *in, double *in_copy, double *out);
#include "filter.cpp"

#endif

