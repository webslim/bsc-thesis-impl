bool aligned(float *src, int stride, int block) {
   if ( ((uintptr_t) src) % 16 == 0
        && block  % 4 == 0
        && stride % 4 == 0)
      return true;
   return false;
}

bool aligned(double *src, int stride, int block) {
   if ( ((uintptr_t) src) % 16 == 0
        && block  % 2 == 0
        && stride % 2 == 0)
      return true;
   return false;
}
