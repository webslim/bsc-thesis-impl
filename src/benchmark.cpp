#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "randomize.h"

/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

static int n_runs = 1;	// number of test runs, option -n

int main(int argc, char **argv) {
	int opt;
	while ((opt = getopt(argc, argv, "n:")) != -1) {
		switch (opt) {
			case 'n':
				n_runs = atoi(optarg);
				break;
		}
	}
	if (n_runs < 1 ) {
		std::cerr << "error invalid run count" << std::endl;
		return EXIT_FAILURE;
	}
   FP_TYPE *in   = new FP_TYPE[INPUT_MAX],
           *copy = new FP_TYPE[INPUT_MAX],
           *out  = new FP_TYPE[INPUT_MAX];
	seed_rng();
	for (int run = 0; run < n_runs; ++run) {
		test(in, copy, out);
	}
   delete [] in;
   delete [] copy;
   delete [] out;
   return EXIT_SUCCESS;
}

