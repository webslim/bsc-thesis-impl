/* C functions */
extern "C" {
   // reduce: original function
   static double reduce(double *in, int len) {
      double result = 0;
      int i;
      for (i = 0; i < len; i++)
         result += in[i];
      return result;
   }

   // reduce: strided C version
   static double reduce_stride(double* in, int len, int stride) {
      double result = 0;
      int i;
      for (i = 0; i < len; i++)
         result += in[i*stride];
      return result;
   }

   // reduce: blockstrided C version
   static double reduce_blockstride(double* in, 
         int len, int block, int stride) {
      double result = 0;
      int i;
      for (i = 0; i < len; i++)
         result += in[i/block*stride + i%block];
      return result;
   }
}

namespace {
#ifdef CLASS
   // reduce accessing input array through a class
   double reduce_class(ArrayAccess& in, int len) {
      double result = 0;
      int i;
      for (i = 0; i < len; i++)
         result += in[i];
      return result;
   }
#endif

   // reduce: C++ version using templates and iterators
   template <typename in_iterator>
   double reduce_iterator(in_iterator& in, int len) {
      double result = 0;
      int i;
      for (i = 0; i < len; i++)
         result += in[i];
      return result;
   }
}

/* run all tests for reduce */
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "reduce"
   std::forward_list<double> results;

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( results.push_front( reduce(in, new_size) ); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   results.push_front( reduce(copy, new_size) ); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( results.push_front( reduce_stride(in, new_size, stride) ); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided(in, stride);
         RUN_TEST( results.push_front( reduce_class(strided, new_size) ); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         RUN_TEST( results.push_front( reduce_iterator(inS, new_size) ); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE(
            results.push_front( reduce_iterator(inCS COMMA new_size) ); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( results.push_front( reduce(in, new_size) ); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      results.push_front( reduce(copy, new_size) ); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST(
               results.push_front(
                  reduce_blockstride(in, new_size, block, stride)
               ); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided(in, stride, block);
            RUN_TEST(
               results.push_front(
                  reduce_class(blockstrided, new_size)
               ); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            RUN_TEST(
               results.push_front(
                  reduce_iterator(inBS, new_size)
               ); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE(
               results.push_front(
                  reduce_iterator(inCBS COMMA new_size)
               ); )
         )
      )
   )
#undef NAME
}

