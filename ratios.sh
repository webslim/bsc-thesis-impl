#!/bin/sh
FILTER="-gcc-Ofast-"
SIZE="800000"

rm -v ratios/*
for data in output_processed/*$FILTER*; do
	size=$SIZE
	# if there is a fixed size, use it instead of $SIZE
	if ! grep -q "^$size," < "$data"; then
		size=`tail -n+2 < "$data" | cut -d, -f1 | sort | uniq | head -n1`
	fi
	func=${data##*/}
	func=${func%.csv}
	func=`echo "$func" | sed "s/${FILTER}.*//"`
	linear=` grep "^$size," < "$data" | cut -d, -f2`
	static=` grep "^$size," < "$data" | cut -d, -f4`
	dynamic=`grep "^$size," < "$data" | cut -d, -f6`
	manual=` grep "^$size," < "$data" | cut -d, -f8`
	copy=`   grep "^$size," < "$data" | cut -d, -f10`
	if [ -z $copy ]; then
		copyratio=-1.0
	else
		copyratio=$copy/$manual
	fi
	echo | awk "{printf \"%f,%f,%f,%f\n\", \
			$linear/$manual, $copyratio, $dynamic/$manual, $static/$manual}" \
			>> ratios/octave-${func}.csv \
			|| exit 1
done

echo "\
Function,\
linear,\
copy,\
dynamic,\
static\
" > ratios/SUMMARY.csv

for infile in ratios/octave-*.csv; do
	func=${infile##*/octave-}
	func=${func%.csv}
	outfile=ratios/${func}.csv
	echo "$func" | sed -E 's/_/\\_/g;s/(.*)/{\\texttt{\1}}/' | tr "\n" "," \
		>> ratios/SUMMARY.csv
	octave <<EOF
values = csvread("$infile");
means = mean(values, "g");
format short;
dlmwrite("$outfile", means, "precision", "%.2f");

EOF
	rm ${infile}
	cat $outfile >> ratios/SUMMARY.csv
done


