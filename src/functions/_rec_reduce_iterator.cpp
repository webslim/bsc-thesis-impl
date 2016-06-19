template <typename in_iterator>
double rec_reduce_iterator(in_iterator x, int size) {
	double tmp1, tmp2;
	if (size == 1) {
		return x[0];
	} else if (size == 2) {
		return x[0] + x[1];
	} else {
		tmp1 = rec_reduce_iterator(x, size/2);
		tmp2 = rec_reduce_iterator(x + size/2,
				size/2 + size%2);
		return tmp1+tmp2;
	}
}
