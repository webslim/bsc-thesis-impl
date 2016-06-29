#!/bin/sh

# this script runs all the steps necessary to generate the
# benchmark plots

./make.sh
./run.sh
./filter_output.sh
./plot.sh
./compare.sh
./ratios.sh

