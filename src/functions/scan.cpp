/* C functions */
extern "C" {
   // scan: original function
   static void scan(double *in, double *out, int len) {
      int i;
      out[0] = 0;
      for (i = 1; i < len; i++)
         out[i] = out[i-1] + in[i-1];
   }

   // scan: strided C version
   static void scan_stride(double* in, double* out, int len, int stride) {
      int i;
      out[0] = 0;
      for(i = 1; i < len; i++)
         out[i * stride] = out[(i-1) * stride] + in[stride * (i-1)];
   }

   // scan: blockstrided C version
   static void scan_blockstride(double* in, double* out, int len, int block, int stride) {
      int i;
      out[0] = 0;
      for(i = 1; i < len; i++)
         out[i / block * stride + i % block] =
            out[(i-1) / block * stride + (i-1) % block] +
             in[(i-1) / block * stride + (i-1) % block];
   }
}

/* C++ functions */
namespace {
#ifdef CLASS
   // scan accessing arrays through a class
   static void scan_class(ArrayAccess& in, ArrayAccess& out, int len) {
      out[0] = 0;
      for (int i = 1; i < len; i++)
         out[i] = out[i-1] + in[i-1];
   }
#endif

   // scan: C++ version using templates and iterators
   template <typename in_iter, typename out_iter>
   static void scan_iterator(in_iter& in, out_iter& out, int len) {
      out[0] = 0;
      for (int i = 1; i < len; i++)
         out[i] = out[i-1] + in[i-1];
   }
}

// run all tests for scan
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "scan"

   ITERATE_INPUT_SIZES(
      
      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( scan(in, out, new_size); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   scan(copy, out, new_size);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( scan_stride(in, out, new_size, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( scan_class(strided_in, strided_out, new_size); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( scan_iterator(inS, outS, new_size); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( scan_iterator(inCS COMMA outCS COMMA new_size); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( scan(in, out, new_size); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      scan(copy, out, new_size);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( scan_blockstride(in, out, new_size, block, stride); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST( scan_class(blockstrided_in, blockstrided_out, new_size); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( scan_iterator(inBS, outBS, new_size); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE( scan_iterator(inCBS COMMA outCBS COMMA new_size); )
         )
      )
   )
#undef NAME
}

