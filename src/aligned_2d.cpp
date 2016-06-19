bool aligned2d(float *src, int width, int height,
		int stride, int block) {
   if (block % 4 != 0 || stride % 4 != 0)
      return false;
   for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; x += 4) {
         int index = x + y*width;
         index = index/block*stride + index%block;
         if ((uintptr_t) &src[index] % 16 != 0)
            return false;
      }
   }
   return true;
}

bool aligned2d(double *src, int width, int height,
		int stride, int block) {
   if (block % 2 != 0 || stride % 2 != 0)
      return false;
   for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; x += 2) {
         int index = x + y*width;
         index = index/block*stride + index%block;
         if ((uintptr_t) &src[index] % 16 != 0)
            return false;
      }
   }
   return true;
}
