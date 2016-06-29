#!/bin/sh

# this script converts the filtered results into a suitable
# form and plots them if $1 != "noplot"

rm output_processed/*
rm plots/*

function sort_functions {
   local sorted=""
   local remaining="$@"
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    linear`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v linear`
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    static`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v static`
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    dynamic`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v dynamic`
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    manual`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v manual`
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    copy`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v copy`
   sorted="${sorted} `echo ${remaining} | tr ' ' '\n' | grep    class`"
   remaining=`        echo ${remaining} | tr ' ' '\n' | grep -v class`

   [ ! -z "${remaining}" ] && sorted="${sorted} ${remaining}"
   echo ${sorted}
}

AWK_MAX='BEGIN {max = 0.0} {if ($1 > max) max=$1} END {if (max-1 < 0) printf "%.6f\n", max+0.001; else printf "%.6f\n", max+0.5}'
AWK_MEAN='BEGIN {sum = 0.0; count = 0} {count++; sum += $1} END {printf "%.6f\n", (sum/count)}'
AWK_DEV='{res = (($2 - $1) ** 2); printf "%.6f\n", res}'
AWK_SUM='{printf "%.6f\n", ($1 + $2)}'
AWK_SQR_DIV='{printf "%.6f\n", sqrt($1 / ($2-1))}'
AWK_OFFSET='{printf "%.6f\n", ($1 * $2 / sqrt($3))}'

# https://en.wikipedia.org/wiki/Student's_t-distribution#Table_of_selected_values
T95[1]=6.314
T95[2]=2.92
T95[3]=2.353
T95[4]=2.132
T95[5]=2.015
T95[6]=1.943
T95[7]=1.895
T95[8]=1.86
T95[9]=1.833

function round_y_max {
   local max_one=`< "$1" cut -d',' -f6 | awk "${AWK_MAX}"`
   local file2=""
   if echo "$1" | grep -q gcc; then
      file2=`echo "$1" | sed 's/gcc/clang/'`
   else
      file2=`echo "$1" | sed 's/clang/gcc/'`
   fi
	if [ ! -f ${file2} ]; then
	        echo -n $max_one
	        return
	fi
   local max_two=`< "${file2}" cut -d',' -f6 | awk "${AWK_MAX}"`
   echo -e "$max_one\n$max_two" | awk "${AWK_MAX}"
}

PLOT_FILTER=""
for file in output_filtered/*${PLOT_FILTER}*.csv; do
   OUTFILE="output_processed/${file#*/}"
   echo "[processing]: ${OUTFILE}"

   FUNCTIONS="`cut -d',' -f 5 ${file} | sort | uniq`"
   FUNCTIONS=`sort_functions ${FUNCTIONS}`
   HEAD="input_size,`echo ${FUNCTIONS} | tr ' ' ',' | sed 's/,/,error,/g'`,error"
   INPUT_SIZES="`cut -d',' -f 1 ${file} | sort -n | uniq`"
   
   echo "${HEAD}" >> "${OUTFILE}"
   for i in ${INPUT_SIZES}; do
      LINE="$i"
      for f in ${FUNCTIONS}; do
         VALS="`grep "^${i},.*${f}," ${file} | cut -d',' -f 6`"
			MEAN=`echo $VALS | tr ' ' '\n' | awk "$AWK_MEAN"`
			N=`echo $VALS | tr ' ' '\n' | wc -l`
			SUM="0.0"
			for val in $VALS; do
				TMP=`echo $val $MEAN | awk "$AWK_DEV"`
				SUM=`echo $SUM $TMP | awk "$AWK_SUM"`
			done
			S=`echo $SUM $N | awk "$AWK_SQR_DIV"`
			# OFFSET is the distance from MEAN to bounds of confidence interval
			OFFSET=`echo "${T95[$((N-1))]} $S $N" | awk "$AWK_OFFSET"`
         LINE="${LINE},${MEAN},${OFFSET}"
      done
      echo "${LINE}" >> "${OUTFILE}"
   done

	[ "$1" == "noplot" ] && continue

   PLOTFILE="${file#*/}"
   PLOTFILE="plots/${PLOTFILE%.csv}.pdf"
   echo "[plotting] ${PLOTFILE}"
   J="`echo ${HEAD} | tr ',' ' ' |  wc -w`"
   YMAX=`round_y_max "${file}"`
   gnuplot > "${PLOTFILE}" <<EOF
set datafile separator ","
set term pdfcairo font "Liberation Mono,10"
set title "${OUTFILE}" noenhanced
set xlabel "array size"
set ylabel "time [ms]"
set style data histogram
set style histogram errorbars linewidth 0.5
set style histogram errorbars gap 2
set style fill solid border -1
set boxwidth 0.9
set xtic rotate by -45 scale 0
set key left top
set grid y
set yrange [0:${YMAX}]
plot for [i=2:${J}:2] '${OUTFILE}' using i:i+1:xticlabels(1) ti columnheader(i)

EOF
done

[ "$1" == "noplot" ] || echo "plots are in plots/"

