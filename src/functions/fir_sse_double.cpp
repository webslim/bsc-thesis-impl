#include <xmmintrin.h>
/* C functions */
extern "C" {
#include "_fir_sse_double_original.c"

   static void fir_sse_double_stride(double *in, double *out, int len, int stride) {
      int i = 0;
      __m128d x, y, i0;
      __m128d two = _mm_set_pd(2.0, 2.0);

      for (; i < len-3; i+=2) {
         // operand one: load
         i0 = _mm_loadh_pd(i0, in + (i*stride));
         i0 = _mm_loadl_pd(i0, in + (i+1)*stride);
         x = i0;
         // operand two: load
         i0 = _mm_loadh_pd(i0, in + (i+1)*stride);
         i0 = _mm_loadl_pd(i0, in + (i+2)*stride);
         // operand two: ready
         y = i0;
         // calculate
         x = _mm_add_pd(x, y);
         x = _mm_div_pd(x, two);
         // store results
         _mm_storeh_pd(out + (i*stride), x);
         _mm_storel_pd(out + (i+1)*stride, x);
      }
      for (; i < len-1; ++i)
         out[i*stride] = (in[(i+1)*stride] + in[i*stride])/2;
   }

   static void fir_sse_double_blockstride(double *in, double *out, int len, int stride, int block) {
      int i = 0;
      __m128d x, y, i0;
      __m128d two = _mm_set_pd(2.0, 2.0);

      for (; i < len-3; i+=2) {
         // operand one: load
         i0 = _mm_loadh_pd(i0, in + (i/block * stride + i%block));
         i0 = _mm_loadl_pd(i0, in + ((i+1)/block*stride + (i+1)%block));
         x = i0;
         // operand two: load
         i0 = _mm_loadh_pd(i0, in + ((i+1)/block*stride + (i+1)%block));
         i0 = _mm_loadl_pd(i0, in + ((i+2)/block*stride + (i+1)%block));
         // operand two: ready
         y = i0;
         // calculate
         x = _mm_add_pd(x, y);
         x = _mm_div_pd(x, two);
         // store results
         _mm_storeh_pd(out + (i/block * stride + i%block), x);
         _mm_storel_pd(out + ((i+1)/block*stride + i%block), x);
      }
      for (; i < len-1; ++i)
         out[i/block*stride + i%block] = (in[(i+1)/block*stride + i%block]
               + in[i/block*stride + i%block])/2;
   }

   static void fir_sse_double_blockstride_aligned(
         double *in, double *out, int len, int stride, int block) {
      int i = 0;
      __m128d i0, i1;
      __m128d two = _mm_set_pd(2.0, 2.0);

      for (; i < len-3; i+=2) {
         i0 = _mm_load_pd(in + (i/block*stride + i%block));
         i1 = _mm_loadu_pd(in + ((i+1)/block*stride + (i+1)%block));
         i0 = _mm_add_pd(i0, i1);
         i0 = _mm_div_pd(i0, two);
         _mm_store_pd(out + (i/block*stride + i%block), i0);
      }
      for (; i < len-1; ++i)
         out[i/block*stride] = (in[(i+1)/block*stride + (i+1)%block]
               + in[i/block*stride + i%block]) / 2;
   }
}

/* C++ functions */
namespace {
#include "_fir_sse_double_iterator.cpp"
}

/* run all tests for fir_sse_double */
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "fir_sse_double"

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( fir_sse_double(in, out, new_size); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   fir_sse_double(copy, out, new_size);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( fir_sse_double_stride(in, out, new_size, stride); )
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( fir_sse_double_iterator(inS, outS, new_size); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( fir_sse_double_iterator(inCS COMMA outCS COMMA new_size); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( fir_sse_double(in, out, new_size); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      fir_sse_double(copy, out, new_size);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            if (aligned(in, stride, block) && aligned(out, stride, block)) {
               RUN_TEST( fir_sse_double_blockstride_aligned(in, out, new_size, stride, block); )
            } else {
               RUN_TEST( fir_sse_double_blockstride(in, out, new_size, stride, block); )
            }
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            if (aligned(in, stride, block) && aligned(out, stride, block)) {
               BlockStridedAligned<double> inBS(in, stride, block);
               BlockStridedAligned<double> outBS(out, stride, block);
               RUN_TEST( fir_sse_double_iterator(inBS, outBS, new_size); )
            } else {
               BlockStrided<double> inBS(in, stride, block);
               BlockStrided<double> outBS(out, stride, block);
               RUN_TEST( fir_sse_double_iterator(inBS, outBS, new_size); );
            }
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE( fir_sse_double_iterator(inCBS COMMA outCBS COMMA new_size); )
         )
      )
   )
#undef NAME
}

