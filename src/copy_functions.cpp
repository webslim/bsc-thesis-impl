// copy for manual striding, input
template <typename F>
static void stride_copy_in(F *in, F *copy, int l, int stride) {
   int i;
   for (i = 0; i < l; i++)
      copy[i] = in[i * stride];
}

// copy for manual striding, output
template <typename F>
static void stride_copy_out(F *out, F *copy, int l, int stride) {
   int i;
   for (i = 0; i < l; i++)
      copy[i*stride] = out[i];
}

// copy for manual blockstriding, input
template <typename F>
static void blockstride_copy_in(F *in, F *copy, int l, int stride, int block) {
   int i;
   for (i = 0; i < l; i++)
      copy[i] = in[(i / block) * stride + i % block];
}

// copy for manual blockstriding, output
template <typename F>
static void blockstride_copy_out(F *out, F *copy, int l, int stride, int block) {
   int i;
   for (i = 0; i < l; i++)
      copy[(i / block) * stride + i % block] = out[i];
}
