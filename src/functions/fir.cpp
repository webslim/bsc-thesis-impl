/* C functions */
extern "C" {
   // fir: original C function
#include "_fir_original.c"

   // fir: strided C version
   static void fir_stride(double *in, double *out, int len, int stride) {
      for(int i = 0; i < len - 1; i++) {
         out[i * stride] = (in[stride * (i + 1)] + in[stride * (i)]) / 2;
      }
   }

   // fir: blockstrided C version
   static void fir_blockstride(double *in, double *out, int len, int stride, int block) {
      int index0, index1;
      for(int i = 0; i < len - 1; i++) {
         index0 = i / block * stride + i % block;
         index1 = (i+1) / block * stride + (i+1) % block;
         out[index0] = (in[index1] + in[index0]) / 2;
      }
   }
}

/* C++ functions */
namespace {
#ifdef CLASS
   // fir accessing arrays through a class
   void fir_class(ArrayAccess& in, ArrayAccess& out, int len) {
      for (int i = 0; i < len-1; i++)
         out[i] = (in[i+1] + in[i])/2;
   }
#endif

   // fir: C++ version using templates and compile-time/run-time iterators
#include "_fir_iterator.cpp"
}

/* run all tests for fir */
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "fir"

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( fir(in, out, new_size); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   fir(copy, out, new_size);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( fir_stride(in, out, new_size, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( fir_class(strided_in, strided_out, new_size); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( fir_iterator(inS, outS, new_size); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( fir_iterator(inCS COMMA outCS COMMA new_size); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( fir(in, out, new_size); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      fir(copy, out, new_size);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( fir_blockstride(in, out, new_size, stride, block); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST( fir_class(blockstrided_in, blockstrided_out, new_size); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( fir_iterator(inBS, outBS, new_size); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE( fir_iterator(inCBS COMMA outCBS COMMA new_size); )
         )
      )
   )
#undef NAME
}

