namespace {

   void contrast_sse_aligned(const double* const input, double *output,
        int width, int height, const double new_max) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x += 4) {
            __m128d a = _mm_load_pd(input+x+y*width);
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max, max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x += 4) {
            __m128d val = _mm_load_pd(input+x+y*width);
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            _mm_store_pd(output+x+y*width, tmp);
         }
      }
   }

   void contrast_sse_unaligned(const double* const input, double *output,
        int width, int height, const double new_max) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x += 4) {
            //__m128d a = _mm_load_pd(input+x+y*width);
            __m128d a;
            a = _mm_loadh_pd(a, input+(x+y*width));
            a = _mm_loadl_pd(a, input+(x+y*width+1));
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max, max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x += 4) {
            //__m128d val = _mm_load_pd(input+x+y*width);
            __m128d val;
            val = _mm_loadh_pd(val, input+(x+y*width));
            val = _mm_loadl_pd(val, input+(x+y*width+1));
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            //_mm_store_pd(output+x+y*width, tmp);
            _mm_storeh_pd(output+(x+y*width), tmp);
            _mm_storel_pd(output+(x+y*width+1), tmp);
         }
      }
   }

   void contrast_sse_stride(const double* const input, double * output,
        int width, int height, const double new_max, int stride) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            __m128d a;
            a = _mm_loadh_pd(a, input+(x+y*width)*stride);
            a = _mm_loadl_pd(a, input+(x+y*width+1)*stride);
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max,max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            __m128d val;
            val = _mm_loadh_pd(val, input+(x+y*width)*stride);
            val = _mm_loadl_pd(val, input+(x+y*width+1)*stride);
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            _mm_storeh_pd(output+(x+y*width)*stride, tmp);
            _mm_storel_pd(output+(x+y*width+1)*stride, tmp);
         }
      }
   }

   void contrast_sse_blockstride_aligned(const double* const input, double * output,
        int width, int height, const double new_max, int stride, int block) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      int index;
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            index = (x+y*width);
            index = index/block*stride + index%block;
            __m128d a = _mm_load_pd(input+index);
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max,max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            index = (x+y*width);
            index = index/block*stride + index%block;
            __m128d val = _mm_load_pd(input+index);
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            _mm_store_pd(output+index, tmp);
         }
      }
   }

   void contrast_sse_blockstride_unaligned(const double* const input, double * output,
        int width, int height, const double new_max, int stride, int block) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      int index;
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            index = (x+y*width);
            index = index/block*stride + index%block;
            __m128d a;
            a = _mm_loadh_pd(a, input+index);
            a = _mm_loadl_pd(a, input+index+1);
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max,max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            index = (x+y*width);
            index = index/block*stride + index%block;
            __m128d val;
            val = _mm_loadh_pd(val, input+index);
            val = _mm_loadl_pd(val, input+index+1);
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            _mm_storeh_pd(output+index, tmp);
            _mm_storel_pd(output+index+1, tmp);
         }
      }
   }

   template <typename in_iter, typename out_iter>
   void contrast_sse_iterator(in_iter& input, out_iter& output,
        int width, int height, double new_max) {
      __m128d new_max_sse = _mm_set1_pd(new_max);
      __m128d max = _mm_set1_pd(0);
      __m128d min = _mm_set1_pd(0);
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            __m128d a = _mm_load_pd(input+x+y*width);
            max = _mm_max_pd(max, a);
            min = _mm_min_pd(min, a);
         }
      }
      __m128d tmp = _mm_shuffle_pd(max,max, 2);
      max = _mm_max_pd(max, tmp);

      tmp = _mm_shuffle_pd(min,min, 2);
      min = _mm_min_pd(min, tmp);
      
      for(int y = 0; y < height; y++) {
         for(int x = 0; x < width; x += 4) {
            __m128d val = _mm_load_pd(input+x+y*width);
            tmp = _mm_mul_pd(_mm_div_pd(_mm_sub_pd(val, min), _mm_sub_pd(max,min)), new_max_sse);
            _mm_store_pd(output+x+y*width, tmp);
         }
      }
   }

}

static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "contrast_sse"
   double new_max = 0.5;
   int side;

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         side = (int) sqrt(new_size);

         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         if (aligned2d(in, side, side) && aligned2d(out, side, side)) {
            RUN_TEST( contrast_sse_aligned(in, out, side, side, new_max); )
         } else {
            RUN_TEST( contrast_sse_unaligned(in, out, side, side, new_max); )
         }
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         if (aligned2d(copy, side, side) && aligned2d(out, side, side)) {
            RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                      contrast_sse_aligned(copy, out, side, side, new_max);
                      stride_copy_out(out, copy, new_size, stride); )
         } else {
            RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                      contrast_sse_unaligned(copy, out, side, side, new_max);
                      stride_copy_out(out, copy, new_size, stride); )
         }
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( contrast_sse_stride(in, out, side, side, new_max, stride); )
#ifdef CLASS
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( contrast_sse_iterator(inS, outS, side, side, new_max); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( contrast_sse_iterator(inCS COMMA outCS COMMA
                               side COMMA side COMMA new_max); )

         ITERATE_BLOCKSIZES(
            side = (int) sqrt(new_size);
            
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            if (aligned2d(in, side, side) && aligned2d(out, side, side)) {
               RUN_TEST( contrast_sse_aligned(in, out, side, side, new_max); )
            } else {
               RUN_TEST( contrast_sse_unaligned(in, out, side, side, new_max); )
            }
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            if (aligned2d(copy, side, side) && aligned2d(out, side, side)) {
               RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                         contrast_sse_aligned(copy, out, side, side, new_max);
                         blockstride_copy_out(out, copy, new_size, stride, block); )
            } else {
               RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                         contrast_sse_unaligned(copy, out, side, side, new_max);
                         blockstride_copy_out(out, copy, new_size, stride, block); )
            }
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            if (aligned2d(in, side, side, stride, block)
               && aligned2d(out, side, side, stride, block)) {
               RUN_TEST( contrast_sse_blockstride_aligned(in, out, side, side, new_max, stride, block); )
            } else {
               RUN_TEST( contrast_sse_blockstride_unaligned(in, out, side, side, new_max, stride, block); )
            }
#ifdef CLASS
#endif
            // ... iterator template dynamic
               print_start(NAME, "iterator-blockstrided-dynamic", input_size, stride, block);
            if (aligned2d(in, side, side, stride, block)
               && aligned2d(out, side, side, stride, block)) {
               BlockStridedAligned<double> inBS(in, stride, block);
               BlockStridedAligned<double> outBS(out, stride, block);
               RUN_TEST( contrast_sse_iterator(inBS, outBS, side, side, new_max); )
               // print_start(NAME, "iterator-blockstrided-dynamic-unaligned",
               //    input_size, stride, block);
               // BlockStrided<double> inBSu(in, stride, block);
               // BlockStrided<double> outBSu(out, stride, block);
               // RUN_TEST( contrast_sse_iterator(inBSu, outBSu, side, side, new_max); )
            } else {
               BlockStrided<double> inBS(in, stride, block);
               BlockStrided<double> outBS(out, stride, block);
               RUN_TEST( contrast_sse_iterator(inBS, outBS, side, side, new_max); )
            }
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static", input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D( contrast_sse_iterator(inCBS
                     COMMA outCBS COMMA side COMMA side COMMA new_max); )
         )
      )
   )
#undef NAME
}


