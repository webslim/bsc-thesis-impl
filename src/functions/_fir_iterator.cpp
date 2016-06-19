template <typename in_iter, typename out_iter>
void fir_iterator(in_iter& in, out_iter& out, int len) {
	for (int i = 0; i < len-1; i++)
		out[i] = (in[i+1] + in[i])/2;
}
