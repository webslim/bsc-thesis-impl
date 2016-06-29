#!/bin/sh

# this script runs all the steps necessary to generate the
# benchmark plots or ratios

# only compute ratios for gcc:
./make.sh noclang
./run.sh gcc
./filter_output.sh
./plot.sh noplot
./ratios.sh

## generate thesis data and compute ratios for gcc
#./make.sh
#./run.sh
#./filter_output.sh
#./plot.sh
#./compare.sh
#./ratios.sh

