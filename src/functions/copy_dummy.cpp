/* C functions */
extern "C" {
   // copy_dummy: original C function
   static void copy_dummy(double *in, double *out, int len) {
      for (int i = 0; i < len; i++)
         out[i] = in[i];
   }

   // copy_dummy: strided C version
   static void copy_dummy_stride(double *in, double *out, int len, int stride) {
      for (int i = 0; i < len; i++) {
         out[i * stride] = in[i*stride];
      }
   }

   // copy_dummy: blockstrided C version
   static void copy_dummy_blockstride(double *in, double *out, int len, int stride, int block) {
      int index;
      for(int i = 0; i < len; i++) {
         index = i/block*stride + i%block;
         out[index] = in[index];
      }
   }
}

/* C++ functions */
namespace {
#ifdef CLASS
   // copy_dummy accessing arrays through a class
   void copy_dummy_class(ArrayAccess& in, ArrayAccess& out, int len) {
      for (int i = 0; i < len; i++)
         out[i] = in[i];
   }
#endif

   // copy_dummy: C++ version using templates and compile-time/run-time iterators
   template <typename in_iter, typename out_iter>
   void copy_dummy_iterator(in_iter& in, out_iter& out, int len) {
      for (int i = 0; i < len; i++)
         out[i] = in[i];
   }
}

/* run all tests for copy_dummy */
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "copy_dummy"

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( copy_dummy(in, out, new_size); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   copy_dummy(copy, out, new_size);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( copy_dummy_stride(in, out, new_size, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( copy_dummy_class(strided_in, strided_out, new_size); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( copy_dummy_iterator(inS, outS, new_size); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( copy_dummy_iterator(inCS COMMA outCS COMMA new_size); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( copy_dummy(in, out, new_size); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      copy_dummy(copy, out, new_size);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( copy_dummy_blockstride(in, out, new_size, stride, block); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST( copy_dummy_class(blockstrided_in, blockstrided_out, new_size); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( copy_dummy_iterator(inBS, outBS, new_size); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE( copy_dummy_iterator(inCBS COMMA outCBS COMMA new_size); )
         )
      )
   )
#undef NAME
}

