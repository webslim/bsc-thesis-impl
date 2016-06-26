#!/bin/sh
FILTER="-gcc-Ofast-"
SIZE="160000"

for data in output_processed/*$FILTER*; do
	size=$SIZE
	if ! grep -q "^$size," < "$data"; then
		size=`tail -n+2 < "$data" | cut -d, -f1 | sort | uniq | head -n1`
	fi
	linear=` grep "^$size," < "$data" | cut -d, -f2`
	static=` grep "^$size," < "$data" | cut -d, -f4`
	dynamic=`grep "^$size," < "$data" | cut -d, -f6`
	manual=` grep "^$size," < "$data" | cut -d, -f8`
	copy=`   grep "^$size," < "$data" | cut -d, -f10`
	echo | awk "{printf \"%f,%f,%f\n\", \
			$manual/$linear, $dynamic/$linear, $static/$linear}" || exit 1
done > ratios.csv

echo "ratios: (manual, dynamic, static) / linear"

octave ratios.m

