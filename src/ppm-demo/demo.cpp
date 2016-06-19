#include "../iterators.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

// modify one colour channel
template <typename iterator>
void modify(iterator& pixels, size_t size) {
	for (size_t i = 0; i < size; ++i)
		//pixels[i] = (char) (pixels[i] * 3.0);
		pixels[i] *= 3.0;
}

int main(void) {
	string line;
	string word;
	size_t width = 0, height = 0, max_color = 0, size = 0;
	istringstream iss;
	streampos bin_start = 0, bin_end = 0, bin_size = 0;
	char *pixels;
	fstream input, output;

	// open input file
	input.open("./tue-pano.ppm", ios::in | ios::binary);
	assert(input.is_open());

	// check magic number
	getline(input, line);
	assert(line == "P6");

	// ignore comments
	do { line.clear(); getline(input, line);
		} while (line[0] == '#');

	// read width of image
	iss.str(line);
	iss >> word;
	width = stoi(word);
	word.clear();

	// read height of image
	iss >> word;
	height = stoi(word);
	word.clear();
	assert(width != 0 && height != 0);
	line.clear();
	size = width * height;

	// read maximum value for one color channel
	getline(input, line);
	max_color = stoi(line);
	line.clear();
	assert(max_color != 0);
	if (max_color > 255) {
		cerr << "sorry, this program only supports a bit depth of 8" << endl;
		return EXIT_FAILURE;
	}

	// get start and end of binary data
	bin_start = input.tellg();
	input.seekg(ios::cur, ios::end);
	bin_end = input.tellg();
	assert(bin_start < bin_end);
	bin_size = bin_end - bin_start;
	assert(size*3 == static_cast<size_t>(bin_size) - 1);
	
	// copy pixel data to memory and close input file
	input.seekg(bin_start);
	pixels = new char[bin_size];
	input.read(pixels, bin_size);
	input.close();

	// do something to the red color channel (rgb)
	CStrided<char, 3> access(pixels);
	modify(access, size);

	// open output file
	output.open("./tue-pano_mod.ppm", ios::out | ios::binary);
	assert(output.is_open());

	// write PPM header
	output << "P6" << endl;
	output << "# image content modified using a striding iterator" << endl;
	output << width << " " << height << endl;
	output << max_color << endl;

	// write pixel data and close output file
	output.write(pixels, bin_size);
	output.close();
	delete [] pixels;

	return EXIT_SUCCESS;
}
