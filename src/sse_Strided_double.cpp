__m128d _mm_load_pd(Strided<double> access) {
   __m128d ret;
   ret = _mm_loadh_pd(ret, &access[0]);
   ret = _mm_loadl_pd(ret, &access[1]);
   return ret;
}
__m128d _mm_loadu_pd(Strided<double> access) {
   return _mm_load_pd(access);
}
void _mm_store_pd(Strided<double> access, __m128d x) {
   _mm_storeh_pd(&access[0], x);
   _mm_storel_pd(&access[1], x);
}
void _mm_storeu_pd(Strided<double> access, __m128d x) {
   _mm_store_pd(access, x);
}
