void fir_sse_double(double *in, double *out, int len) {
	int i = 0;
	__m128d i0, i1;
	__m128d two = _mm_set_pd(2.0, 2.0);

	for (; i < len-3; i+=2) {
		i0 = _mm_load_pd(in + i);
		i1 = _mm_loadu_pd(in + (i+1));
		i0 = _mm_add_pd(i0, i1);
		i0 = _mm_div_pd(i0, two);
		_mm_store_pd(out + i, i0);
	}
	for (; i < len-1; ++i)
		out[i] = (in[i+1] + in[i])/2;
}
