template <typename in_iter, typename out_iter>
void fir_sse_float_iterator(in_iter& in, out_iter& out,
		int len) {
	int i = 0;
	__m128 i0, i1, two = _mm_set_ps1(2.0);
	for (; i < len-5; i+=4) {
		i0 = _mm_load_ps(in + i);
		i1 = _mm_loadu_ps(in + (i+1));
		i0 = _mm_add_ps(i0, i1);
		i0 = _mm_div_ps(i0, two);
		_mm_store_ps(out + i, i0);
	}
	for (; i < len-1; ++i)
		out[i] = (in[i+1] + in[i])/2;
}
