/* C functions */
extern "C" {
#include "_fir_sse_float_original.c"

   static void fir_sse_float_stride(float *in, float *out, int len, int stride) {
      int i = 0;
      __m128 x, y, i0, i1, i2, i3;
      __m128 two = _mm_set_ps1(2.0);

      for (; i < len-5; i+=4) {
         // operand one: load
         i0 = _mm_load_ss(in+  i  *stride);
         i1 = _mm_load_ss(in+(i+1)*stride);
         i2 = _mm_load_ss(in+(i+2)*stride);
         i3 = _mm_load_ss(in+(i+3)*stride);
         // operand one: shuffle
         i0 = _mm_shuffle_ps(i0, i1, 0);
         i2 = _mm_shuffle_ps(i2, i3, 0);
         i0 = _mm_shuffle_ps(i0, i2, 136); // 1000 1000: elements 2 and 0 of each
         // operand one: x ready
         x = i0;
         // operand two: load
         i0 = _mm_load_ss(in+(i+1)*stride);
         i1 = _mm_load_ss(in+(i+2)*stride);
         i2 = _mm_load_ss(in+(i+3)*stride);
         i3 = _mm_load_ss(in+(i+4)*stride);
         // operand two: shuffle
         i0 = _mm_shuffle_ps(i0, i1, 0);
         i2 = _mm_shuffle_ps(i2, i3, 0);
         i0 = _mm_shuffle_ps(i0, i2, 136); // 1000 1000: elements 2 and 0 of each
         // operand two: ready
         y = i0;
         // calculate
         x = _mm_add_ps(x, y);
         x = _mm_div_ps(x, two);
         // store results
         _mm_store_ss(out+  i  *stride, x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out+(i+1)*stride, x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out+(i+2)*stride, x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out+(i+3)*stride, x);
      }
      for (; i < len-1; ++i)
         out[i*stride] = (in[(i+1)*stride] + in[i*stride])/2;
   }

   static void fir_sse_float_blockstride(float *in, float *out, int len, int stride, int block) {
      int i = 0;
      __m128 x, y, i0, i1, i2, i3;
      __m128 two = _mm_set_ps1(2.0);

      for (; i < len-5; i+=4) {
         // operand one: load
         i0 = _mm_load_ss(in + (i/block*stride + i%block));
         i1 = _mm_load_ss(in + ((i+1)/block*stride + (i+1)%block));
         i2 = _mm_load_ss(in + ((i+2)/block*stride + (i+2)%block));
         i3 = _mm_load_ss(in + ((i+3)/block*stride + (i+3)%block));
         // operand one: shuffle
         i0 = _mm_shuffle_ps(i0, i1, 0);
         i2 = _mm_shuffle_ps(i2, i3, 0);
         i0 = _mm_shuffle_ps(i0, i2, 136); // 1000 1000: elements 2 and 0 of each
         // operand one: x ready
         x = i0;
         // operand two: load
         i0 = _mm_load_ss(in + ((i+1)/block*stride + (i+1)%block));
         i1 = _mm_load_ss(in + ((i+2)/block*stride + (i+2)%block));
         i2 = _mm_load_ss(in + ((i+3)/block*stride + (i+3)%block));
         i3 = _mm_load_ss(in + ((i+4)/block*stride + (i+4)%block));
         // operand two: shuffle
         i0 = _mm_shuffle_ps(i0, i1, 0);
         i2 = _mm_shuffle_ps(i2, i3, 0);
         i0 = _mm_shuffle_ps(i0, i2, 136); // 1000 1000: elements 2 and 0 of each
         // operand two: ready
         y = i0;
         // calculate
         x = _mm_add_ps(x, y);
         x = _mm_div_ps(x, two);
         // store results
         _mm_store_ss(out + (i/block*stride + i%block), x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out + ((i+1)/block*stride + (i+1)%block), x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out + ((i+2)/block*stride + (i+2)%block), x);
         x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
         _mm_store_ss(out + ((i+3)/block*stride + (i+3)%block), x);
      }
      for (; i < len-1; ++i)
         out[i/block*stride + i%block] =
            (in[(i+1)/block*stride + (i+1)%block]
             + in[i/block*stride + i%block]) / 2;
   }

   static void fir_sse_float_blockstride_aligned(
         float *in, float *out, int len, int stride, int block) {
      int i = 0;
      __m128 i0, i1;
      __m128 two = _mm_set_ps1(2.0);

      for (; i < len-5; i += 4) {
         i0 = _mm_load_ps(in + (i/block*stride + i%block));
         i1 = _mm_loadu_ps(in + ((i+1)/block*stride + (i+1)%block));
         i0 = _mm_add_ps(i0, i1);
         i0 = _mm_div_ps(i0, two);
         _mm_store_ps(out + (i/block*stride + i%block), i0);
      }
      for (; i < len-1; ++i)
         out[i/block*stride + i%block] = (in[(i+1)/block*stride + (i+1)%block]
               + in[i/block*stride + i%block]) / 2;
   }
}

/* C++ functions */
namespace {
#include "_fir_sse_float_iterator.cpp"
}

/* run all tests for fir_sse_float */
static void test(float *in, float *copy, float *out) {
   ITERATION_VARS
#define NAME "fir_sse_float"

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( fir_sse_float(in, out, new_size); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   fir_sse_float(copy, out, new_size);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( fir_sse_float_stride(in, out, new_size, stride); )
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<float> inS(in, stride);
         Strided<float> outS(out, stride);
         RUN_TEST( fir_sse_float_iterator(inS, outS, new_size); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_FLOAT( fir_sse_float_iterator(inCS COMMA outCS COMMA new_size); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( fir_sse_float(in, out, new_size); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      fir_sse_float(copy, out, new_size);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            if (aligned(in, stride, block) && aligned(out, stride, block)) {
               RUN_TEST( fir_sse_float_blockstride_aligned(in, out, new_size, stride, block); )
            } else {
               RUN_TEST( fir_sse_float_blockstride(in, out, new_size, stride, block); )
            }
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            if (aligned(in, stride, block) && aligned(out, stride, block)) {
               BlockStridedAligned<float> inBS(in, stride, block);
               BlockStridedAligned<float> outBS(out, stride, block);
               RUN_TEST( fir_sse_float_iterator(inBS, outBS, new_size); )
            } else {
               BlockStrided<float> inBS(in, stride, block);
               BlockStrided<float> outBS(out, stride, block);
               RUN_TEST( fir_sse_float_iterator(inBS, outBS, new_size); );
            }
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_FLOAT( fir_sse_float_iterator(inCBS COMMA outCBS COMMA new_size); )
         )
      )
   )
#undef NAME
}

