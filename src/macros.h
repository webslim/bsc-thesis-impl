/* test procedure adapted from Beni Hess' paper211.pdf, page 7: */

#ifndef MACROS_H
#define MACROS_H

#include <stdexcept>
#include "alignment.h"
#include "randomize.h"

/* the following macros are used in all function tests */

#ifndef REPETITIONS
#define REPETITIONS 1
#endif

#define STRIDE_MAX 8   // max stride step; blocks are chosen up to this value

#define N_MAX 10       // how many times input size is increased
#define STEP (400 * 400) // increase input size by STEP

#define AVG(t) ((t) / REPETITIONS)    // average time needed for test
#define INPUT_MAX 2097152*STRIDE_MAX // for dft_1048576
//#define INPUT_MAX (N_MAX * STEP)
// calculate time elapsed since start of test in seconds as double
#define SECONDS(t) ((clock() - (t)) / (double) CLOCKS_PER_SEC)

// declare variables needed in test loops
#define ITERATION_VARS                      \
   int input_size, new_size, stride, block; \
   clock_t start;

// variables are in ITERATION_VARS
// loop skips some values
#define ITERATE_BLOCKSIZES(block_loop)                                \
   for (block = stride/2; block > 1 && block < stride; block *= 2) {  \
      /* blockstrided access */                                       \
      new_size = input_size/stride*block + (input_size/stride)%block; \
      block_loop                                                      \
   };

// variables are in ITERATION_VARS
// loop skips some values
#define ITERATE_STRIDES(stride_loop) \
   for (stride = 2; stride <= STRIDE_MAX; stride *= 2) { \
      /* strided access */                               \
      new_size = input_size / stride;                    \
      stride_loop                                        \
   };

// n is declared in ITERATION_VARS
#define ITERATE_INPUT_SIZES(main_test_loop) \
   for (int n = 1; n <= N_MAX; ++n) {       \
      input_size = STEP * n;                \
      main_test_loop                        \
   };

// repeat REPETITIONS times to calculate average and complete the output line
// must be preceded by a call to print_start (see print.h)
// variables are defined in SET_ITERATION_VARS
// test is an expression or a sequence of expressions
#define RUN_TEST(test) {                           \
	randomize(in, input_size);                   \
   start = clock();                                \
   for (int i = 0; i < REPETITIONS; ++i)  { test } \
   print_stop(SECONDS(start));                     \
}

#define COMMA ,

#define RUN_CSTRIDED_TEST_FLOAT(func) \
   if (stride == 2) {                 \
      CStrided<float, 2> inCS(in);    \
      CStrided<float, 2> outCS(out) ; \
      RUN_TEST( func )                \
   } else if (stride == 4) {          \
      CStrided<float, 4> inCS(in);    \
      CStrided<float, 4> outCS(out) ; \
      RUN_TEST( func )                \
   } else if (stride == 8) {          \
      CStrided<float, 8> inCS(in);    \
      CStrided<float, 8> outCS(out) ; \
      RUN_TEST( func )                \
   } else                             \
      throw std::out_of_range(        \
         "missing case in fixed template iterator test"); \

#define RUN_CSTRIDED_TEST_DOUBLE(func) \
   if (stride == 2) {                  \
      CStrided<double, 2> inCS(in);    \
      CStrided<double, 2> copyCS(copy); \
      CStrided<double, 2> outCS(out) ; \
      RUN_TEST( func )                 \
   } else if (stride == 4) {           \
      CStrided<double, 4> inCS(in);    \
      CStrided<double, 4> copyCS(copy); \
      CStrided<double, 4> outCS(out) ; \
      RUN_TEST( func )                 \
   } else if (stride == 8) {           \
      CStrided<double, 8> inCS(in);    \
      CStrided<double, 8> copyCS(copy); \
      CStrided<double, 8> outCS(out) ; \
      RUN_TEST( func )                 \
   } else                              \
      throw std::out_of_range(         \
         "missing case in fixed template iterator test"); \

#define RUN_CBLOCKSTRIDED_TEST_FLOAT(func)                      \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
         CBlockStrided<float, 2, 4> inCBS(in);                  \
         CBlockStrided<float, 2, 4> outCBS(out);                \
         RUN_TEST( func )                                       \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
         if (aligned(in COMMA stride COMMA block)               \
               && aligned(out COMMA stride COMMA block)) {      \
            CBlockStridedAligned<float, 4, 8> inCBS(in);        \
            CBlockStridedAligned<float, 4, 8> outCBS(out);      \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<float, 4, 8> inCBS(in);               \
            CBlockStrided<float, 4, 8> outCBS(out);             \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

#define RUN_CBLOCKSTRIDED_TEST_FLOAT_2D(func)                   \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
         CBlockStrided<float, 2, 4> inCBS(in);                  \
         CBlockStrided<float, 2, 4> outCBS(out);                \
         RUN_TEST( func )                                       \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
         if (aligned2d(in COMMA side COMMA side COMMA stride COMMA block) \
               && aligned2d(out COMMA side COMMA side COMMA stride COMMA block)) { \
            CBlockStridedAligned<float, 4, 8> inCBS(in);        \
            CBlockStridedAligned<float, 4, 8> outCBS(out);      \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<float, 4, 8> inCBS(in);               \
            CBlockStrided<float, 4, 8> outCBS(out);             \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

#define RUN_CBLOCKSTRIDED_TEST_DOUBLE(func)                     \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
         if (aligned(in COMMA stride COMMA block)               \
               && aligned(out COMMA stride COMMA block)) {      \
            CBlockStridedAligned<double, 2, 4> inCBS(in);       \
            CBlockStridedAligned<double, 2, 4> outCBS(out);     \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<double, 2, 4> inCBS(in);              \
            CBlockStrided<double, 2, 4> outCBS(out);            \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
         if (aligned(in COMMA stride COMMA block)               \
               && aligned(out COMMA stride COMMA block)) {      \
            CBlockStridedAligned<double, 4, 8> inCBS(in);       \
            CBlockStridedAligned<double, 4, 8> outCBS(out);     \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<double, 4, 8> inCBS(in);              \
            CBlockStrided<double, 4, 8> outCBS(out);            \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

// TODO: the alignment test should be outside of this
#define RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D(func)                  \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
         if (aligned2d(in COMMA side COMMA side COMMA stride COMMA block)          \
               && aligned2d(out COMMA side COMMA side COMMA stride COMMA block)) { \
            CBlockStridedAligned<double, 2, 4> inCBS(in);       \
            CBlockStridedAligned<double, 2, 4> outCBS(out);     \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<double, 2, 4> inCBS(in);              \
            CBlockStrided<double, 2, 4> outCBS(out);            \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
         if (aligned2d(in COMMA side COMMA side COMMA stride COMMA block) \
               && aligned2d(out COMMA side COMMA side COMMA stride COMMA block)) { \
            CBlockStridedAligned<double, 4, 8> inCBS(in);       \
            CBlockStridedAligned<double, 4, 8> outCBS(out);     \
            RUN_TEST( func )                                    \
         } else {                                               \
            CBlockStrided<double, 4, 8> inCBS(in);              \
            CBlockStrided<double, 4, 8> outCBS(out);            \
            RUN_TEST( func )                                    \
         }                                                      \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

#define RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D_ALIGNED(func)          \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
            CBlockStridedAligned<double, 2, 4> copyCBS(copy);   \
            CBlockStridedAligned<double, 2, 4> inCBS(in);       \
            CBlockStridedAligned<double, 2, 4> outCBS(out);     \
            RUN_TEST( func )                                    \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
            CBlockStridedAligned<double, 4, 8> copyCBS(copy);   \
            CBlockStridedAligned<double, 4, 8> inCBS(in);       \
            CBlockStridedAligned<double, 4, 8> outCBS(out);     \
            RUN_TEST( func )                                    \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

#define RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D_UNALIGNED(func)        \
   if (stride == 4) {                                           \
      if (block == 2) {                                         \
            CBlockStrided<double, 2, 4> copyCBS(copy);          \
            CBlockStrided<double, 2, 4> inCBS(in);              \
            CBlockStrided<double, 2, 4> outCBS(out);            \
            RUN_TEST( func )                                    \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else if (stride == 8) {                                    \
      if (block == 4) {                                         \
            CBlockStrided<double, 4, 8> copyCBS(copy);          \
            CBlockStrided<double, 4, 8> inCBS(in);              \
            CBlockStrided<double, 4, 8> outCBS(out);            \
            RUN_TEST( func )                                    \
      } else                                                    \
         throw std::out_of_range(                               \
            "missing case in fixed template iterator test");    \
   } else                                                       \
      throw std::out_of_range(                                  \
         "missing case in fixed template iterator test");

#endif

