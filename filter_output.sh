#!/bin/sh

# this script separates output data by access pattern

STRIDES="2 4 8" # see src/marcros.h

rm output_filtered/*

for output in output/*.csv; do
   NAME="${output##*/}"
   NAME="${NAME%.csv}"
	for s in ${STRIDES}; do
		grep -E "^[0-9]*,${s},0" "${output}" \
			> output_filtered/"${NAME}-stride-${s}.csv"
		case "$NAME" in
			rec_reduce*) continue ;;
		esac
		let b=${s}/2
		(( ${b} < 2 )) && continue
		grep -E "^[0-9]*,${s},${b}" "${output}" \
			> output_filtered/"${NAME}-stride-${s}-block-${b}.csv"
	done
done

echo "filtered output is in output_filtered/"
