void fir(double *in, double *out, int len) {
	for (int i = 0; i < len-1; i++)
		out[i] = (in[i+1] + in[i])/2;
}
