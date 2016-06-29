#ifndef SCAN_H
#define SCAN_H

#define FP_TYPE double

#ifdef CLASS
#include "../array_class.h"
#endif
#include "../copy_functions.h"
#include "../iterators.h"
#include "../macros.h"
#include "../print.h"

static void test(double *in, double *in_copy, double *out);
#include "scan.cpp"

#endif

