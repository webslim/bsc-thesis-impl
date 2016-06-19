#ifndef CONTRAST_H
#define CONTRAST_H

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
#include "contrast.cpp"

#endif

