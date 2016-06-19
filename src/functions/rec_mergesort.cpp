extern "C" {
   static void rec_mergesort(double *in, double *merge, double *out,
         int left, int right) {
      int mid, i, l, r;
      if (left < right) { // more than 1 element left
         mid = (left + right) / 2;
         rec_mergesort(in, merge, out, left, mid);
         rec_mergesort(in, merge, out, mid+1, right);

         // merge phase
         l = left;
         r = mid+1;
         i = 0;
         while (l <= mid && r <= right) {
            if (in[l] < in[r])
               merge[i++] = in[l++];
            else
               merge[i++] = in[r++];
         }
         while (r <= right)
            merge[i++] = in[r++];
         while (l <= mid)
            merge[i++] = in[l++];
         for (i = 0; i <= right-left; i++)
            out[left+i] = merge[i];
      }
   }

   // (access to merge is still linear because it is not used further...)
   static void rec_mergesort_stride(double *in, double *merge, double *out,
         int left, int right, int stride) {
      int mid, i, l, r;
      if (left < right) { // more than 1 element left
         mid = (left + right) / 2;
         rec_mergesort_stride(in, merge, out, left, mid, stride);
         rec_mergesort_stride(in, merge, out, mid+1, right, stride);

         // merge phase
         l = left;
         r = mid+1;
         i = 0;
         while (l <= mid && r <= right) {
            if (in[l*stride] < in[r*stride])
               merge[i++] = in[l++ * stride];
            else
               merge[i++] = in[r++ * stride];
         }
         while (r <= right)
            merge[i++] = in[r++ * stride];
         while (l <= mid)
            merge[i++] = in[l++ * stride];
         for (i = 0; i <= right-left; i++)
            out[(left+i) * stride] = merge[i];
      }
   }

   static void rec_mergesort_blockstride(double *in, double *merge,
         double *out, int left, int right, int stride, int block) {
      int mid, i, l, r;
      if (left < right) { // more than 1 element left
         mid = (left + right) / 2;
         rec_mergesort_blockstride(in, merge, out, left, mid, stride, block);
         rec_mergesort_blockstride(in, merge, out, mid+1, right, stride, block);

         // merge phase
         l = left;
         r = mid+1;
         i = 0;
         while (l <= mid && r <= right) {
            if (in[l/block*stride + l%block] < in[r/block*stride + r%block]) {
               merge[i++] = in[l/block*stride + l%block];
               ++l;
            } else {
               merge[i++] = in[r/block*stride + r%block];
               ++r;
            }
         }
         while (r <= right) {
            merge[i++] = in[r/block*stride + r%block];
            ++r;
         }
         while (l <= mid) {
            merge[i++] = in[l/block*stride + l%block];
            ++l;
         }
         for (i = 0; i <= right-left; i++)
            out[(left+i)/block*stride + (left+i)%block] = merge[i];
      }
   }
}

namespace {
#ifdef CLASS
   void rec_mergesort_class(ArrayAccess& in, double *merge, ArrayAccess& out,
         int left, int right) {
      int mid, i, l, r;
      if (left < right) { // more than 1 element left
         mid = (left + right) / 2;
         rec_mergesort_class(in, merge, out, left, mid);
         rec_mergesort_class(in, merge, out, mid+1, right);

         // merge phase
         l = left;
         r = mid+1;
         i = 0;
         while (l <= mid && r <= right) {
            if (in[l] < in[r])
               merge[i++] = in[l++];
            else
               merge[i++] = in[r++];
         }
         while (r <= right)
            merge[i++] = in[r++];
         while (l <= mid)
            merge[i++] = in[l++];
         for (i = 0; i <= right-left; i++)
            out[left+i] = merge[i];
      }
   }
#endif

   template <typename in_iter, typename out_iter>
   void rec_mergesort_iterator(in_iter& in, double *merge, out_iter& out,
         int left, int right) {
      int mid, i, l, r;
      if (left < right) { // more than 1 element left
         mid = (left + right) / 2;
         rec_mergesort_iterator(in, merge, out, left, mid);
         rec_mergesort_iterator(in, merge, out, mid+1, right);

         // merge phase
         l = left;
         r = mid+1;
         i = 0;
         while (l <= mid && r <= right) {
            if (in[l] < in[r])
               merge[i++] = in[l++];
            else
               merge[i++] = in[r++];
         }
         while (r <= right)
            merge[i++] = in[r++];
         while (l <= mid)
            merge[i++] = in[l++];
         for (i = 0; i <= right-left; i++)
            out[left+i] = merge[i];
      }
   }
}

static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "rec_mergesort"

   ITERATE_INPUT_SIZES(

      ITERATE_STRIDES(
         // original version, linear access
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST( rec_mergesort(in, copy, out, 0, new_size-1); )
         // ... copy
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST( stride_copy_in(in, copy, new_size, stride);
                   rec_mergesort(in, copy, out, 0, new_size-1);
                   stride_copy_out(out, copy, new_size, stride); )
         // ... manual C
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST( rec_mergesort_stride(in, copy, out, 0, new_size-1, stride); )
#ifdef CLASS
         // ... class
         print_start(NAME, "class-strided", input_size, stride, 0);
         StridedArrayAccess strided_in(in, stride);
         StridedArrayAccess strided_out(out, stride);
         RUN_TEST( rec_mergesort_class(strided_in, copy, strided_out, 0, new_size-1); )
#endif
         // ... iterator template dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(in, stride);
         Strided<double> outS(out, stride);
         RUN_TEST( rec_mergesort_iterator(inS, copy, outS, 0, new_size-1); )
         // ... iterator template static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE( rec_mergesort_iterator(inCS COMMA copy COMMA outCS
               COMMA 0 COMMA new_size-1); )

         ITERATE_BLOCKSIZES(
            // original version, linear access
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST( rec_mergesort(in, copy, out, 0, new_size-1); )
            // ... copy
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST( blockstride_copy_in(in, copy, new_size, stride, block);
                      rec_mergesort(in, copy, out, 0, new_size-1);
                      blockstride_copy_out(out, copy, new_size, stride, block); )
            // ... manual C
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST( rec_mergesort_blockstride(in, copy, out, 0, new_size-1, stride, block); )
#ifdef CLASS
            // ... class
            print_start(NAME, "class-blockstrided", input_size, stride, block);
            BlockStridedArrayAccess blockstrided_in(in, stride, block);
            BlockStridedArrayAccess blockstrided_out(out, stride, block);
            RUN_TEST( rec_mergesort_class(blockstrided_in, copy, blockstrided_out, 0, new_size-1); )
#endif
            // ... iterator template dynamic
            print_start(NAME, "iterator-blockstrided-dynamic",
               input_size, stride, block);
            BlockStrided<double> inBS(in, stride, block);
            BlockStrided<double> outBS(out, stride, block);
            RUN_TEST( rec_mergesort_iterator(inBS, copy, outBS, 0, new_size-1); )
            // ... iterator template static
            print_start(NAME, "iterator-blockstrided-static",
                  input_size, stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE(
                  rec_mergesort_iterator(inCBS COMMA copy COMMA outCBS
                     COMMA 0 COMMA new_size-1); )
         )
      )
   )
#undef NAME
}
