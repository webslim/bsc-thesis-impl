/* helper functions to benchmark the original function with
 * strided and blockstrided access simulated by copying beforehand
 * and afterwards
 */

#ifndef COPY_FUNCTIONS_H
#define COPY_FUNCTIONS_H

template <typename F>
static void stride_copy_in(F *in, F *copy, int l, int stride);

template <typename F>
static void stride_copy_out(F *out, F *copy, int l, int stride);

template <typename F>
static void blockstride_copy_in(F *in, F *copy, int l, int stride, int block);

template <typename F>
static void blockstride_copy_out(F *out, F *copy, int l, int stride, int block);

#include "copy_functions.cpp"

#endif

