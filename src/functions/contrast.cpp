/* C functions */
extern "C" {
   // contrast: original function
   static void contrast(double *in, double *out, int width,
         int height, double new_max) {
      double max = 0,
             min = 0,
             tmp;
      int y, x, index;
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = in[index];
            if (max < tmp)
               max = tmp;
            if (min > tmp)
               min = tmp;
         }
      }
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = (in[index]-min) / (max-min) * new_max;
            out[index] = tmp;
         }
      }
   }

   // contrast: strided C version
   static void contrast_stride(double* in, double* out, int width,
         int height, double new_max, int stride) {
      double max = 0,
             min = 0,
             tmp;
      int y, x, index;
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = stride * (x + y * width);
            tmp = in[index];
            if (max < tmp)
               max = tmp;
            if (min > tmp)
               min = tmp;
         }
      }
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = stride * (x + y * width);
            tmp = (in[index]-min) / (max-min) * new_max;
            out[index] = tmp;
         }
      }
   }

   // contrast: blockstrided C version
   static void contrast_blockstride(double* in, double* out, int width,
         int height, double new_max, int block, int stride) {
      double max = 0,
             min = 0,
             tmp;
      int y, x, index;
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            index = index / block * stride + index % block;
            tmp = in[index];
            if (max < tmp)
               max = tmp;
            if (min > tmp)
               min = tmp;
         }
      }
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            index = index / block * stride + index % block;
            tmp = (in[index]-min) / (max-min) * new_max;
            out[index] = tmp;
         }
      }
   }
}

/* C++ functions */
namespace {
#ifdef CLASS
   // contrast accessing arrays through a class
   static void contrast_class(ArrayAccess& in, ArrayAccess& out, int width,
         int height, double new_max) {
      double max = 0,
             min = 0,
             tmp;
      int y, x, index;
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = in[index];
            if (max < tmp)
               max = tmp;
            if (min > tmp)
               min = tmp;
         }
      }
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = (in[index]-min) / (max-min) * new_max;
            out[index] = tmp;
         }
      }
   }
#endif

   // contrast: C++ version using templates and compile-time/run-time iterators
   template <typename in_iter, typename out_iter>
   static void contrast_iterator(in_iter& in, out_iter& out, int width, int height,
         double new_max) {
      double max = 0,
             min = 0,
             tmp;
      int y, x, index;
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = in[index];
            if (max < tmp)
               max = tmp;
            if (min > tmp)
               min = tmp;
         }
      }
      for (y = 0; y < height; y++) {
         for (x = 0; x < width; x++) {
            index = x + y * width;
            tmp = (in[index]-min) / (max-min) * new_max;
            out[index] = tmp;
         }
      }
   }
}

/* run all tests for contrast */
static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "contrast"
   double new_max = 0.5;
   int side;

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         side = (int) sqrt(new_size);

         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( contrast(in, out, side, side, new_max); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   contrast(copy, out, side, side, new_max);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( contrast_stride(in, out, side, side, new_max, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( contrast_class(strided_in, strided_out, side, side, new_max); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( contrast_iterator(inS, outS, side, side, new_max); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( contrast_iterator(inCS COMMA outCS COMMA
                               side COMMA side COMMA new_max); )

         ITERATE_BLOCKSIZES(
            side = (int) sqrt(new_size);
            
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( contrast(in, out, side, side, new_max); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      contrast(copy, out, side, side, new_max);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( contrast_blockstride(in, out, side, side, new_max, block, stride); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST( contrast_class(blockstrided_in, blockstrided_out,
                         side, side, new_max); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( contrast_iterator(inBS, outBS, side, side, new_max); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE( contrast_iterator(inCBS
                     COMMA outCBS COMMA side COMMA side COMMA new_max); )
         )
      )
   )
#undef NAME
}

