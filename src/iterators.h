// very specialized iterator
// the standard interface would be:
// http://www.cplusplus.com/reference/iterator/RandomAccessIterator/

#ifndef ITERATORS_H
#define ITERATORS_H

#include <emmintrin.h>
#include <cstdint>

// when pointer arithmetic is used, the iterator has to be passed by value
// else it can be passed by reference

// stride specified at runtime
#include "iterator_strided_runtime.cpp"
// stride specified at compile time
#include "iterator_strided_static.cpp"

// blockstride specified at runtime
#include "iterator_blockstrided_runtime.cpp"
// BlockStridedAligned: optimized SSE loading
template <typename T>
class BlockStridedAligned {
   T* array;
   int stride,
       block;
public:
   BlockStridedAligned<T>(T* _array, int _stride, int _block):
      array(_array), stride(_stride), block(_block) {}
   T& operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   BlockStridedAligned<T> operator+(int offset) {
      return BlockStridedAligned<T>(
            array + (offset/block*stride + offset%block),
            stride, block);
   }
};

// blockstride specified at compile time
#include "iterator_blockstrided_static.cpp"
// CBlockStridedAligned: optimized SSE loading
template <typename T, int block, int stride>
class CBlockStridedAligned {
   T* array;
public:
   CBlockStridedAligned<T, block, stride>(T* _array): array(_array) {}
   T& operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   CBlockStridedAligned<T, block, stride> operator+(int offset) {
      return CBlockStridedAligned<T, block, stride>(
            array + (offset/block*stride + offset%block));
   }
};

/* * * * * * * * * * * * * * *
 * overloaded SSE functions  *
 * * * * * * * * * * * * * * */

/* SSE: Strided */
// single precision
#include "sse_Strided_float.cpp"
// double precision
#include "sse_Strided_double.cpp"

/* SSE: CStrided */
// single precision
template <int stride>
__m128 _mm_load_ps(CStrided<float, stride> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136); // 1000 1000: elements 2 and 0 of each
}
template <int stride>
__m128 _mm_loadu_ps(CStrided<float, stride> access) {
   return _mm_load_ps(access);
}
template <int stride>
void _mm_store_ps(CStrided<float, stride> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original, optional)
}
template <int stride>
void _mm_storeu_ps(CStrided<float, stride> access, __m128 x) {
   _mm_store_ps(access, x);
}
// double precision
template <int stride>
__m128d _mm_load_pd(CStrided<double, stride> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
template <int stride>
__m128d _mm_loadu_pd(CStrided<double, stride> access) {
   return _mm_load_pd(access);
}
template <int stride>
void _mm_store_pd(CStrided<double, stride> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}
template <int stride>
void _mm_storeu_pd(CStrided<double, stride> access, __m128d x) {
   _mm_store_pd(access, x);
}

/* SSE: BlockStrided */
// single precision
__m128 _mm_load_ps(BlockStrided<float> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136); // 1000 1000: elements 2 and 0 of each
}
__m128 _mm_loadu_ps(BlockStrided<float> access) {
   return _mm_load_ps(access);
}
void _mm_store_ps(BlockStrided<float> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original, optional)
}
void _mm_storeu_ps(BlockStrided<float> access, __m128 x) {
   _mm_store_ps(access, x);
}
// double precision
__m128d _mm_load_pd(BlockStrided<double> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
__m128d _mm_loadu_pd(BlockStrided<double> access) {
   return _mm_load_pd(access);
}
void _mm_store_pd(BlockStrided<double> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}
void _mm_storeu_pd(BlockStrided<double> access, __m128d x) {
   _mm_store_pd(access, x);
}

/* SSE: BlockStridedAligned */
// single precision
#include "sse_BlockStridedAligned_float.cpp"
// double precision
#include "sse_BlockStridedAligned_double.cpp"

/* SSE: CBlockStrided */
// single precision
template <int block, int stride>
__m128 _mm_load_ps(CBlockStrided<float, block, stride> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136); // 1000 1000: elements 2 and 0 of each
}
template <int block, int stride>
__m128 _mm_loadu_ps(CBlockStrided<float, block, stride> access) {
   return _mm_load_ps(access);
}
template <int block, int stride>
void _mm_store_ps(CBlockStrided<float, block, stride> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original)
}
template <int block, int stride>
void _mm_storeu_ps(CBlockStrided<float, block, stride> access, __m128 x) {
   _mm_store_ps(access, x);
}
// double precision
template <int block, int stride>
__m128d _mm_load_pd(CBlockStrided<double, block, stride> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
template <int block, int stride>
__m128d _mm_loadu_pd(CBlockStrided<double, block, stride> access) {
   return _mm_load_pd(access);
}
template <int block, int stride>
void _mm_store_pd(CBlockStrided<double, block, stride> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}
template <int block, int stride>
void _mm_storeu_pd(CBlockStrided<double, block, stride> access, __m128d x) {
   _mm_store_pd(access, x);
}

/* SSE: CBlockStridedAligned */
// single precision
template <int block, int stride>
__m128 _mm_load_ps(CBlockStridedAligned<float, block, stride> access) {
   return _mm_load_ps(&access[0]);
}
template <int block, int stride>
__m128 _mm_loadu_ps(CBlockStridedAligned<float, block, stride> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136); // 1000 1000: elements 2 and 0 of each
}
template <int block, int stride>
void _mm_store_ps(CBlockStridedAligned<float, block, stride> access, __m128 x) {
   _mm_store_ps(&access[0], x);
}
template <int block, int stride>
void _mm_storeu_ps(CBlockStridedAligned<float, block, stride> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original)
}
// double precision
template <int block, int stride>
__m128d _mm_load_pd(CBlockStridedAligned<double, block, stride> access) {
   return _mm_load_pd(&access[0]);
}
template <int block, int stride>
__m128d _mm_loadu_pd(CBlockStridedAligned<double, block, stride> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
template <int block, int stride>
void _mm_store_pd(CBlockStridedAligned<double, block, stride> access, __m128d x) {
   _mm_store_pd(&access[0], x);
}
template <int block, int stride>
void _mm_storeu_pd(CBlockStridedAligned<double, block, stride> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}

#endif

