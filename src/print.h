/* functions used for printing benchmark data */
#ifndef PRINT_H
#define PRINT_H

#include <iostream>

static void print_start(std::string function, std::string version,
      int input_size, int stride, int block);
static void print_stop(double time);
#include "print.cpp"

#endif

