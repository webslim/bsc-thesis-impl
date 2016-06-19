// print current test
static void print_start(std::string function, std::string version,
                        int input_size, int stride, int block) {
   std::cout << input_size << ','
             << stride     << ','
             << block      << ','
             << function   << ','
             << version    << ',';
}

// print results
static void print_stop(double time) {
   size_t old = std::cout.precision();
   std::cout.precision(12);
   std::cout <<  AVG(time)*1000 << std::endl;
   std::cout.precision(old);
}

