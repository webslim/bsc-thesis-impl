#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <cstdint>

template <typename T> bool aligned(T *src) {
   return (uintptr_t) src % 16 == 0;
}

#include "aligned_1d.cpp"

bool aligned2d(float *src, int width, int height) {
   for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; x += 4) {
         if ((uintptr_t) (src + x + y*width) % 16 != 0)
            return false;
      }
   }
   return true;
}

bool aligned2d(double *src, int width, int height) {
   for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; x += 2) {
         if ((uintptr_t) (src + x + y*width) % 16 != 0)
            return false;
      }
   }
   return true;
}

#include "aligned_2d.cpp"

#endif

