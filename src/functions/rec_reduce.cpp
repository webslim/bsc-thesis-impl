extern "C" {
#include "_rec_reduce_original.c"

   static double rec_reduce_stride(double *x, int size, int stride) {
      double tmp1,tmp2;
      if (size == 1) {
         return x[0];
      } else if (size == 2) {
         return x[0] + x[stride];
      } else {
         tmp1 = rec_reduce_stride(x, size/2, stride);
         tmp2 = rec_reduce_stride(x + size/2 * stride,
               size/2 + size%2, stride);
         return tmp1 + tmp2;
      }
   }

//   static double rec_reduce_blockstride(double *x, int size,
//         int block, int stride, int pos) {
//      double tmp1,tmp2;
//      int increment;
//      if (size == 1) {
//         return x[0];
//      } else if (size == 2) {
//         return x[0] + x[((pos+1) % block == 0) ? stride : 1];
//      } else {
//         tmp1 = rec_reduce_blockstride(x, size/2, stride, block, pos);
//         increment = (size/2) / block * stride + (size/2)%block;
//         tmp2 = rec_reduce_blockstride(x + increment,
//               size/2 + size%2, stride, block, pos + increment);
//         return tmp1 + tmp2;
//      }
//   }
}

namespace {
#ifdef CLASS
   double rec_reduce_class(ArrayAccess& x, int size, int pos) {
      double tmp1, tmp2;
      if (size == 1) {
         return x[pos];
      } else if (size == 2) {
         return x[pos] + x[pos+1];
      } else {
         tmp1 = rec_reduce_class(x, size/2, pos);
         tmp2 = rec_reduce_class(x, size/2 + size%2, pos + size/2);
         return tmp1 + tmp2;
      }
   }
#endif

#include "_rec_reduce_iterator.cpp"
}

static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "rec_reduce"
   std::forward_list<double> results;

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( results.push_front( rec_reduce(in, new_size) ); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   results.push_front( rec_reduce(copy, new_size) ); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( results.push_front( rec_reduce_stride(in, new_size, stride) ); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         RUN_TEST( results.push_front( rec_reduce_class(strided_in, new_size, 0) ); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         RUN_TEST( results.push_front( rec_reduce_iterator(inS, new_size) ); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE(
            results.push_front( rec_reduce_iterator(inCS COMMA new_size) ); )

/*
         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( results.push_front( rec_reduce(in, new_size) ); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      results.push_front( rec_reduce(copy, new_size) ); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST(
               results.push_front(
                  rec_reduce_blockstride(in, new_size, block, stride, 0)
               ); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            RUN_TEST(
               results.push_front(
                  rec_reduce_class(blockstrided_in, new_size, 0)
               ); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            RUN_TEST(
               results.push_front(
                  rec_reduce_iterator(inBS, new_size)
               ); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE(
               results.push_front(
                  rec_reduce_iterator(inCBS COMMA new_size)
               ); )
         )
*/
      )
   )
#undef NAME
}

