/* C functions */
extern "C" {
   // filter: original function
   static void filter(double *in, double *out, double *kernel,
         int width, int height, int kernel_width, int kernel_height) {
      int half_width = kernel_width >> 1,
          half_height = kernel_height >> 1,
          y, x, index, dy, dx;
      double tmp;
      for (y = half_height; y < height-half_height; y++) {
         for (x = half_width; x < width-half_width; x++) {
            tmp = 0;
            index = y + x * width;
            for (dy = -half_height; dy <= half_height; dy++)
               for (dx = -half_width; dx <= half_width; dx++)
                  tmp += in[index+dx+dy*width] *
                            kernel[(dy+half_height)*kernel_width +
                            dx + half_width];
            out[index] = tmp;
         }
      }
   }

   // filter: strided C version
   static void filter_stride(double* in, double* out, double *kernel,
         int width, int height, int kernel_width, int kernel_height, int stride) {
      int half_width = kernel_width >> 1,
          half_height = kernel_height >> 1,
          y, x, index, dy, dx;
      double tmp;
      for (y = half_height; y < height-half_height; y++) {
         for (x = half_width; x < width-half_width; x++) {
            tmp = 0;
            index = y + x * width;
            for (dy = -half_height; dy <= half_height; dy++)
               for (dx = -half_width; dx <= half_width; dx++)
                  tmp += in[stride*(index+dx+dy*width)] *
                     kernel[(dy+half_height)*kernel_width + dx + half_width];
            out[index * stride] = tmp;
         }
      }
   }

   // filter: blockstrided C version
   static void filter_blockstride(double* in, double* out, double *kernel, int width,
         int height, int kernel_width, int kernel_height, int block, int stride) {
      int half_width = kernel_width >> 1,
          half_height = kernel_height >> 1,
          y, x, index, dy, dx, i;
      double tmp;
      for (y = half_height; y < height-half_height; y++) {
         for (x = half_width; x < width-half_width; x++) {
            tmp = 0;
            index = y+x*width;
            for (dy = -half_height; dy <= half_height; dy++)
               for (dx = -half_width; dx <= half_width; dx++) {
                  i = index + dx + dy * width;
                  tmp += in[i/block*stride + i%block] *
                     kernel[(dy+half_height)*kernel_width + dx + half_width];
               }
            out[index / block * stride + index % block] = tmp;
         }
      }
   }
}

namespace {
#ifdef CLASS
   // filter accessing input array through a class
   static void filter_class(ArrayAccess& in, ArrayAccess& out, double *kernel,
         int width, int height, int kernel_width, int kernel_height) {
      int half_width = kernel_width >> 1,
          half_height = kernel_height >> 1,
          y, x, index, dy, dx;
      double tmp;
      for (y = half_height; y < height-half_height; y++) {
         for (x = half_width; x < width-half_width; x++) {
            tmp = 0;
            index = y + x * width;
            for (dy = -half_height; dy <= half_height; dy++)
               for (dx = -half_width; dx <= half_width; dx++)
                  tmp += in[index+dx+dy*width] *
                            kernel[(dy+half_height)*kernel_width +
                            dx + half_width];
            out[index] = tmp;
         }
      }
   }
#endif

   // filter: C++ version using templates and compile-time/run-time iterators
   template <typename in_iter, typename out_iter>
   static void filter_iterator(in_iter& in, out_iter& out, double *kernel,
         int width, int height, int kernel_width, int kernel_height) {
      int half_width = kernel_width >> 1,
          half_height = kernel_height >> 1,
          y, x, index, dy, dx;
      double tmp;
      for (y = half_height; y < height-half_height; y++) {
         for (x = half_width; x < width-half_width; x++) {
            tmp = 0;
            index = y + x * width;
            for (dy = -half_height; dy <= half_height; dy++)
               for (dx = -half_width; dx <= half_width; dx++)
                  tmp += in[index+dx+dy*width] *
                            kernel[(dy+half_height)*kernel_width +
                            dx + half_width];
            out[index] = tmp;
         }
      }
   }
}

// run all tests for filter
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "filter"
   double kernel[9]; // 3x3
   int kernel_width = 3, kernel_height = 3, side;
   randomize(kernel, 9);

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         side = (int) sqrt(new_size);

         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( filter(in, out, kernel, side, side, kernel_width, kernel_height); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   filter(copy, out, kernel, side, side,
                      kernel_width, kernel_height);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( filter_stride(in, out, kernel, side, side,
               kernel_width, kernel_height, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( filter_class(strided_in, strided_out, kernel,
               side, side, kernel_width, kernel_height); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( filter_iterator(inS, outS, kernel, side, side,
                  kernel_width, kernel_height); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE(
               filter_iterator(inCS COMMA outCS COMMA kernel COMMA
                  side COMMA side COMMA kernel_width COMMA kernel_height); )

         ITERATE_BLOCKSIZES(
            side = (int) sqrt(new_size);
            
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( filter(in, out, kernel, side, side, kernel_width, kernel_height); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST(
               blockstride_copy_in(in, copy, new_size, stride, block);
               filter(copy, out, kernel, side, side, kernel_width, kernel_height);
               blockstride_copy_out(out, copy, new_size, stride, block);
            )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( filter_blockstride(in, out, kernel, side, side,
                  kernel_width, kernel_height, block, stride); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST(
               filter_class(blockstrided_in, blockstrided_out, kernel, side, side,
                  kernel_width, kernel_height);
            )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( filter_iterator(inBS, outBS, kernel, side, side,
                     kernel_width, kernel_height); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE(
               filter_iterator(inCBS COMMA outCBS COMMA kernel COMMA side
                  COMMA side COMMA kernel_width COMMA kernel_height); )
         )
      )
   )
#undef NAME
}

