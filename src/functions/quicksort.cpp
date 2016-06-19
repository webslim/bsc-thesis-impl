// original function
namespace {
   void swap(double *A, int a, int b) {
      double tmp = A[a];
      A[a] = A[b];
      A[b] = tmp;
   }

   int partition(double *A, int lo, int hi) {
      int pivotIndex = lo + (hi-lo)/2;
      double pivotValue = A[pivotIndex];
      swap(A, pivotIndex, hi);
      int storeIndex = lo;
      for (int i = lo; i < hi; ++i) {
         if (A[i] < pivotValue) {
            swap(A, i, storeIndex);
            ++storeIndex;
         }
      }
      swap(A, storeIndex, hi);
      return storeIndex;
   }

   void quicksort(double *A, int lo, int hi) {
      if (lo < hi) {
         int p = partition(A, lo, hi);
         quicksort(A, lo, p-1);
         quicksort(A, p+1, hi);
      }
   }
}

// manual striding
namespace {
   void swap_stride(double *A, int a, int b, int stride) {
      double tmp = A[a*stride];
      A[a*stride] = A[b*stride];
      A[b*stride] = tmp;
   }

   int partition_stride(double *A, int lo, int hi, int stride) {
      int pivotIndex = lo + (hi-lo)/2;
      double pivotValue = A[pivotIndex*stride];
      swap_stride(A, pivotIndex, hi, stride);
      int storeIndex = lo;
      for (int i = lo; i < hi; ++i) {
         if (A[i*stride] < pivotValue) {
            swap_stride(A, i, storeIndex, stride);
            ++storeIndex;
         }
      }
      swap_stride(A, storeIndex, hi, stride);
      return storeIndex;
   }

   void quicksort_stride(double *A, int lo, int hi, int stride) {
      if (lo < hi) {
         int p = partition_stride(A, lo, hi, stride);
         quicksort_stride(A, lo, p-1, stride);
         quicksort_stride(A, p+1, hi, stride);
      }
   }
}

// manual blockstriding
namespace {
   void swap_blockstride(double *A, int a, int b, int stride, int block) {
      double tmp = A[a/block*stride + a%block];
      A[a/block*stride + a%block] = A[b/block*stride + b%block];
      A[b/block*stride + b%block] = tmp;
   }

   int partition_blockstride(double *A, int lo, int hi, int stride, int block) {
      int pivotIndex = lo + (hi-lo)/2;
      double pivotValue = A[pivotIndex/block*stride + pivotIndex%block];
      swap_blockstride(A, pivotIndex, hi, stride, block);
      int storeIndex = lo;
      for (int i = lo; i < hi; ++i) {
         if (A[i/block*stride + i%block] < pivotValue) {
            swap_blockstride(A, i, storeIndex, stride, block);
            ++storeIndex;
         }
      }
      swap_blockstride(A, storeIndex, hi, stride, block);
      return storeIndex;
   }

   void quicksort_blockstride(double *A, int lo, int hi, int stride, int block) {
      if (lo < hi) {
         int p = partition_blockstride(A, lo, hi, stride, block);
         quicksort_blockstride(A, lo, p-1, stride, block);
         quicksort_blockstride(A, p+1, hi, stride, block);
      }
   }
}

// iterator
namespace {
   template <typename iterator>
   void swap_iterator(iterator& A, int a, int b) {
      double tmp = A[a];
      A[a] = A[b];
      A[b] = tmp;
   }

   template <typename iterator>
   int partition_iterator(iterator& A, int lo, int hi) {
      int pivotIndex = lo + (hi-lo)/2;
      double pivotValue = A[pivotIndex];
      swap_iterator(A, pivotIndex, hi);
      int storeIndex = lo;
      for (int i = lo; i < hi; ++i) {
         if (A[i] < pivotValue) {
            swap_iterator(A, i, storeIndex);
            ++storeIndex;
         }
      }
      swap_iterator(A, storeIndex, hi);
      return storeIndex;
   }

   template <typename iterator>
   void quicksort_iterator(iterator& A, int lo, int hi) {
      if (lo < hi) {
         int p = partition_iterator(A, lo, hi);
         quicksort_iterator(A, lo, p-1);
         quicksort_iterator(A, p+1, hi);
      }
   }
}

static void test(double *in, double *copy, double *out) {
   ITERATION_VARS
#define NAME "quicksort"
// backup input and use only copy in tests!
#define MEMCPY memcpy(copy, in, input_size*sizeof(double))

   ITERATE_INPUT_SIZES (
      
      ITERATE_STRIDES(
         // linear
         print_start(NAME, "linear", input_size, stride, 0);
         RUN_TEST(
            MEMCPY;
            quicksort(copy, 0, new_size-1);
         )
         // copying
         print_start(NAME, "copy-strided", input_size, stride, 0);
         RUN_TEST(
            MEMCPY;
            stride_copy_in(in, copy, new_size, stride);
            quicksort(copy, 0, new_size-1);
         )
         // manual
         print_start(NAME, "manual-strided", input_size, stride, 0);
         RUN_TEST(
            MEMCPY;
            quicksort_stride(copy, 0, new_size-1, stride);
         )
         // iterator dynamic
         print_start(NAME, "iterator-strided-dynamic", input_size, stride, 0);
         Strided<double> inS(copy, stride);
         RUN_TEST(
            MEMCPY;
            quicksort_iterator(inS, 0, new_size-1);
         )
         // iterator static
         print_start(NAME, "iterator-strided-static", input_size, stride, 0);
         RUN_CSTRIDED_TEST_DOUBLE(
            memcpy(out, in, input_size*sizeof(double));
            quicksort_iterator(outCS COMMA 0 COMMA new_size-1); 
         )

         ITERATE_BLOCKSIZES(
            // linear
            print_start(NAME, "linear", input_size, stride, block);
            RUN_TEST(
               MEMCPY;
               quicksort(copy, 0, new_size-1);
            )
            // copying
            print_start(NAME, "copy-blockstrided", input_size, stride, block);
            RUN_TEST(
               MEMCPY;
               blockstride_copy_in(in, copy, new_size, stride, block);
               quicksort(copy, 0, new_size-1);
            )
            // manual
            print_start(NAME, "manual-blockstrided", input_size, stride, block);
            RUN_TEST(
               MEMCPY;
               quicksort_blockstride(copy, 0, new_size-1, stride, block);
            )
            // iterator dynamic
            print_start(NAME, "iterator-blockstrided-dynamic", input_size,
                  stride, block);
            BlockStrided<double> inBS(copy, stride, block);
            RUN_TEST(
                  MEMCPY;
                  quicksort_iterator(inBS, 0, new_size-1);
            )
            // iterator static
            print_start(NAME, "iterator-blockstrided-static", input_size,
                  stride, block);
            RUN_CBLOCKSTRIDED_TEST_DOUBLE(
               memcpy(out, in, input_size*sizeof(double));
               quicksort_iterator(outCBS COMMA 0 COMMA new_size-1);   
            )
         )
      )
   )
#undef NAME
#undef MEMCPY
}

