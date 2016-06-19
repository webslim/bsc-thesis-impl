// very specialized iterator
// the standard interface would be:
// http://www.cplusplus.com/reference/iterator/RandomAccessIterator/

#ifndef ITERATORS_NOREF_H
#define ITERATORS_NOREF_H

#include <emmintrin.h>

// when pointer arithmetic is used, the iterator has to be passed by value
// else it can be passed by reference

// stride specified at runtime
template <typename T>
class Strided {
   T* array;
   int stride;
public:
   Strided<T>(T* _array, int _stride): array(_array), stride(_stride) {}
   T operator[](int index) {
      return array[index * stride];
   }
   Strided<T> operator+(int offset) {
      return Strided<T>(array + offset * stride, stride);
   }
};

// stride specified at compile time
template <typename T, int stride>
class CStrided {
   T* array;
public:
   CStrided<T, stride>(T* _array): array(_array) {}
   T operator[](int index) {
      return array[index * stride];
   }
   CStrided<T, stride> operator+(int offset) {
      return CStrided<T, stride>(array + offset * stride);
   }
};

// blockstride specified at runtime
template <typename T>
class BlockStrided {
   T* array;
   int stride,
       block;
public:
   BlockStrided<T>(T* _array, int _stride, int _block):
      array(_array), stride(_stride), block(_block) {}
   T operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   BlockStrided<T> operator+(int offset) {
      return BlockStrided<T>(
            array + (offset/block*stride + offset%block),
            stride, block);
   }
};

// blockstride specified at compile time
template <typename T, int block, int stride>
class CBlockStrided {
   T* array;
public:
   CBlockStrided<T, block, stride>(T* _array): array(_array) {}
   T operator[](int index) {
      int bnum = index / block;
      return array[bnum * stride + index % block];
   }
   CBlockStrided<T, block, stride> operator+(int offset) {
      return CBlockStrided<T, block, stride>(
            array + (offset/block*stride + offset%block));
   }
};

// for runtime settings
//
// global overloaded sse functions, single precision
template <template <class> class Access>
__m128 _mm_load_ps(Access<float> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136); // 1000 1000: elements 2 and 0 of each
}
template <template <class> class Access>
__m128 _mm_loadu_ps(Access<float> access) { // same as above
   return _mm_load_ps(access);
}
template <template <class> class Access>
void _mm_store_ps(Access<float> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original)
}
// for runtime settings
//
// global overloaded sse functions, double precision
template <template <class> class Access>
__m128d _mm_load_pd(Access<double> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
template <template <class> class Access>
__m128d _mm_loadu_pd(Access<double> access) {
   return _mm_load_pd(access);
}
template <template <class> class Access>
void _mm_store_pd(Access<double> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}

/* there must be a more elegant way for the following */
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
__m128 _mm_loadu_ps(CStrided<float, stride> access) { // same as above
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
   x = _mm_shuffle_ps(x, x, 57); // (restore original)
}
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
__m128 _mm_loadu_ps(CBlockStrided<float, block, stride> access) { // same as above
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

#endif

