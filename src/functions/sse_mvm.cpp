namespace {
   void sse_mvm_aligned(double *x, double *M, double *y, int size) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_load_pd(x+j);
            m1 = _mm_load_pd(M+index);
            m2 = _mm_load_pd(M+index+size);

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_store_pd(y+i, y1);
      }
   }

   void sse_mvm_unaligned(double *x, double *M, double *y, int size) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_loadh_pd(x1, x +     j);
            x1 = _mm_loadl_pd(x1, x + (j+1));
            m1 = _mm_loadh_pd(m1, M +     index);
            m1 = _mm_loadl_pd(m1, M + (index+1));
            m2 = _mm_loadh_pd(m2, M +   (index+size));
            m2 = _mm_loadl_pd(m2, M + (index+size+1));

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_storeh_pd(y +     i, y1);
         _mm_storel_pd(y + (i+1), y1);
      }
   }

   void sse_mvm_stride(double *x, double *M, double *y, int size, int stride) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_loadh_pd(x1, x +     j*stride);
            x1 = _mm_loadl_pd(x1, x + (j+1)*stride);
            m1 = _mm_loadh_pd(m1, M +     index*stride);
            m1 = _mm_loadl_pd(m1, M + (index+1)*stride);
            m2 = _mm_loadh_pd(m2, M +   (index+size)*stride);
            m2 = _mm_loadl_pd(m2, M + (index+size+1)*stride);

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_storeh_pd(y +     i*stride, y1);
         _mm_storel_pd(y + (i+1)*stride, y1);
      }
   }

   void sse_mvm_blockstride_aligned(double *x, double *M, double *y, int size,
         int stride, int block) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_load_pd(x+(j/block*stride + j%block));
            m1 = _mm_load_pd(M+(index/stride*block + index%block));
            m2 = _mm_load_pd(M+((index+size)/block*stride + (index+size)%block));

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_store_pd(y+(i/block*stride + i%block), y1);
      }
   }

   void sse_mvm_blockstride_unaligned(double *x, double *M, double *y, int size,
         int stride, int block) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_loadh_pd(x1, x + (    j/block*stride +     j%block));
            x1 = _mm_loadl_pd(x1, x + ((j+1)/block*stride + (j+1)%block));
            m1 = _mm_loadh_pd(m1, M + (    index/block*stride +     index%block));
            m1 = _mm_loadl_pd(m1, M + ((index+1)/block*stride + (index+1)%block));
            m2 = _mm_loadh_pd(m2, M + (  (index+size)/block*stride +   (index+size)%block));
            m2 = _mm_loadl_pd(m2, M + ((index+size+1)/block*stride + (index+size+1)%block));

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_storeh_pd(y + (    i/block*stride +     i%block), y1);
         _mm_storel_pd(y + ((i+1)/block*stride + (i+1)%block), y1);
      }
   }

   template <typename in_iter, typename out_iter>
   void sse_mvm_iterator(in_iter& x, in_iter& M, out_iter& y, int size) {
      __m128d m1, m2, x1, y1;
      for (int i = 0; i < size; i += 2) {
         y1 = _mm_set1_pd(0);
         for (int j = 0; j < size; j += 2) {
            // y1 = x1 * M + y1, 2x2 blocking              
            int index = i*size+j;
            x1 = _mm_load_pd(x+j);
            m1 = _mm_load_pd(M+index);
            m2 = _mm_load_pd(M+index+size);

            m1 = _mm_mul_pd(m1, x1);
            m2 = _mm_mul_pd(m2, x1);

            m1 = _mm_hadd_pd(m1, m2);
            y1 = _mm_add_pd(y1, m1);
         }
         _mm_store_pd(y+i, y1);
      }
   }
}

/*
 * the copying overwrites values, but what is important is the
 * amount of copying, not the content, because we do not use it for
 * anything and the same operations are applied to everything.
 */

static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "sse_mvm"
   int side;

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         side = (int) sqrt(new_size);

         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         if (aligned(copy) && aligned2d(in, side, side) && aligned(out)) {
            RUN_TEST( sse_mvm_aligned(copy, in, out, side); )
         } else {
            RUN_TEST( sse_mvm_unaligned(copy, in, out, side); )
         }
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         if (aligned(copy) && aligned2d(in, side, side) && aligned(out)) {
            RUN_TEST(
                      stride_copy_in(copy, copy, side, stride);
                      stride_copy_in(in,   copy, new_size, stride);
                      sse_mvm_aligned(copy, in, out, side);
                      stride_copy_out(out,  copy, side, stride); )
         } else {
            RUN_TEST(
                      stride_copy_in(copy, copy, side, stride);
                      stride_copy_in(in,   copy, new_size, stride);
                      sse_mvm_unaligned(copy, in, out, side);
                      stride_copy_out(out, copy, side, stride); )
         }
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( sse_mvm_stride(copy, in, out, side, stride); )
#ifdef CLASS
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> copyS(copy, stride);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( sse_mvm_iterator(copyS, inS, outS, side); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( sse_mvm_iterator(copyCS COMMA inCS COMMA outCS COMMA side); )

         ITERATE_BLOCKSIZES(
            side = (int) sqrt(new_size);
            
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            if (aligned(copy) && aligned2d(in, side, side) && aligned(out)) {
               RUN_TEST( sse_mvm_aligned(copy, in, out, side); )
            } else {
               RUN_TEST( sse_mvm_unaligned(copy, in, out, side); )
            }
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            if (aligned(copy), aligned2d(in, side, side) && aligned(out)) {
               RUN_TEST(
                         blockstride_copy_in(copy, copy, side, stride, block);
                         blockstride_copy_in(in, copy, new_size, stride, block);
                         sse_mvm_aligned(copy, in, out, side);
                         blockstride_copy_out(out, copy, side, stride, block); )
            } else {
               RUN_TEST(
                         blockstride_copy_in(copy, copy, side, stride, block);
                         blockstride_copy_in(in, copy, new_size, stride, block);
                         sse_mvm_unaligned(copy, in, out, side);
                         blockstride_copy_out(out, copy, side, stride, block); )
            }
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            if (aligned(copy, stride, block)
                  && aligned2d(in, side, side, stride, block)
                  && aligned(out, stride, block)) {
               RUN_TEST( sse_mvm_blockstride_aligned(copy, in, out, side, stride, block); )
            } else {
               RUN_TEST( sse_mvm_blockstride_unaligned(copy, in, out, side, stride, block); )
            }
#ifdef CLASS
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic", input_size, stride, block);
            if (aligned(copy, stride, block)
                  && aligned2d(in, side, side, stride, block)
                  && aligned(out, stride, block)) {
               BlockStridedAligned<double> copyBS(copy, stride, block);
               BlockStridedAligned<double> inBS(in, stride, block);
               BlockStridedAligned<double> outBS(out, stride, block);
               RUN_TEST( sse_mvm_iterator(copyBS, inBS, outBS, side); )
            } else {
               BlockStrided<double> copyBS(copy, stride, block);
               BlockStrided<double> inBS(in, stride, block);
               BlockStrided<double> outBS(out, stride, block);
               RUN_TEST( sse_mvm_iterator(copyBS, inBS, outBS, side); )
            }
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static", input_size, stride, block);
            if (aligned(copy, stride, block)
                  && aligned2d(in, side, side, stride, block)
                  && aligned(out, stride, block)) {
               RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D_ALIGNED( sse_mvm_iterator(copyCBS, inCBS
                        COMMA outCBS COMMA side); )
            } else {
               RUN_CBLOCKSTRIDED_TEST_DOUBLE_2D_UNALIGNED( sse_mvm_iterator(copyCBS, inCBS
                        COMMA outCBS COMMA side); )
            }
         )
      )
   )
#undef NAME
}


