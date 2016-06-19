__m128 _mm_load_ps(Strided<float> access) {
   __m128 a0, a1, a2, a3;
   a0 = _mm_load_ss(&access[0]);
   a1 = _mm_load_ss(&access[1]);
   a2 = _mm_load_ss(&access[2]);
   a3 = _mm_load_ss(&access[3]);
   a0 = _mm_shuffle_ps(a0, a1, 0);
   a2 = _mm_shuffle_ps(a2, a3, 0);
   return _mm_shuffle_ps(a0, a2, 136);
	// 1000 1000: elements 2 and 0 of each
}
__m128 _mm_loadu_ps(Strided<float> access) {
   return _mm_load_ps(access);
}
void _mm_store_ps(Strided<float> access, __m128 x) {
   _mm_store_ss(&access[0], x);
   x = _mm_shuffle_ps(x, x, 57); // 0011 1001 -> 0321: rotate
   _mm_store_ss(&access[1], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[2], x);
   x = _mm_shuffle_ps(x, x, 57); // rotate
   _mm_store_ss(&access[3], x);
   x = _mm_shuffle_ps(x, x, 57); // (restore original, optional)
}
void _mm_storeu_ps(Strided<float> access, __m128 x) {
   _mm_store_ps(access, x);
}
