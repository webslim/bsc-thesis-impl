#ifndef RANDOMIZE_H
#define RANDOMIZE_H

#include <ctime>

template <typename F>
static void randomize(F* array, int size) {
   int i;
   for (i = 0; i < size; ++i) // random in range [-1,1]
      array[i] = (random() / (F) RAND_MAX)*2 - 1;
}

// call this once at program start
static void seed_rng() {
   srandom((unsigned int) time(NULL) + clock()); // seed random number generator
}

#endif

