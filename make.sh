#!/bin/sh

# this script compiles the benchmark programs with different
# compilers and optimization flags

rm bin/*

# -DCLASS: build and test access classes with virtual functions
# -DREPETITIONS=<n> to collect an average over n repetitions in one run
#    this is the old way to let the program compute an average time.
#    the script are now updated to support multiple separate runs.
#    input is not randomized again when repeating.
OPTS="-DCLASS -DREPETITIONS=1"

GCC="g++ -std=c++11 -Wall -Wno-uninitialized -march=native -g ${OPTS}"
CLANG="clan${GCC}"
GCC+=" -Wno-unused-but-set-variable"

SRC="src/benchmark.cpp"
OUT="bin/"
FLAGS="Ofast"
FUNCTIONFILTER=""

for func in src/functions/*${FUNCTIONFILTER}*.h; do
	func_name=${func##*/}
	func_name=${func_name%.h}
	for flag in ${FLAGS}; do
		GCC_CMD="${GCC} -${flag} -o ${OUT}${func_name}-gcc-${flag} ${SRC} -include ${func}"
		echo "${GCC_CMD}"
		${GCC_CMD}
		CLANG_CMD="${CLANG} -${flag} -o ${OUT}${func_name}-clang-${flag} ${SRC} -include ${func}"
		echo "${CLANG_CMD}"
		${CLANG_CMD}
	done
done

strip "${OUT}"*
echo "all compilations done"

