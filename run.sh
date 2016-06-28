#!/bin/sh

# this script runs the compiled benchmarks and saves the results

#RUNS="10" # number of test runs, update T-test table in plot.sh for values > 10
RUNS="2" # TODO switch back to 10 when ready
# run only specific versions of the benchmark
RUN_FILTER="*-gcc-*"

rm output/*
for benchmark in bin/${RUN_FILTER}; do
   echo "${benchmark} -n $RUNS > output/${benchmark#*/}.csv"
   ${benchmark} -n $RUNS | tee "output/${benchmark#*/}.csv" \
      | xargs -IS echo -ne "\rS                   " # show current line
   echo -e "\ndone."
done

