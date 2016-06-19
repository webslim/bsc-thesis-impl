#!/bin/sh

# use pdflatex to create side by side views of plots for each function
# for easy comparison

#FLAGS="O2"
FLAGS="Ofast"

rm comparison/*

cd plots
FUNCTIONS=`ls *.pdf | cut -d'-' -f1 | sort | uniq`
cd ..

function combine5 {
   cd comparison
   pdflatex 2>&1 >/dev/null <<EOF
\\documentclass[a4paper]{article}
\\usepackage[top=3.3cm, bottom=1mm, left=2cm, right=2cm]{geometry}
\\usepackage{graphicx}
\\begin{document}
\\noindent
\\centering
\\verb|results: ${11} (-${12})|\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$1}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$6}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$2}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$7}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$3}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$8}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$4}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$9}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$5}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../${10}}}
\\end{document}
EOF
   mv texput.pdf "${11}-${12}.pdf"
   rm texput.*
   cd ..
}

# rec_reduce does not allow blockstriding, compare only 6 plots
function combine3 {
   cd comparison
   pdflatex 2>&1 >/dev/null <<EOF
\\documentclass[a4paper]{article}
\\usepackage[top=3.3cm, bottom=1mm, left=2cm, right=2cm]{geometry}
\\usepackage{graphicx}
\\begin{document}
\\noindent
\\centering
\\verb|results: ${7} (-${8})|\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$1}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$4}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$2}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$5}}\\\\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$3}}\
\\frame{\\includegraphics[width=0.5\\textwidth]{../$6}}\\\\
\\end{document}
EOF
   mv texput.pdf "${7}-${8}.pdf"
   rm texput.*
   cd ..
}

for func in ${FUNCTIONS}; do
   for flag in ${FLAGS}; do
      gcc_funcs=`ls   plots/${func}-gcc-${flag}-*.pdf   | head -n5`
      clang_funcs=`ls plots/${func}-clang-${flag}-*.pdf | head -n5`
		case "$func" in
			rec_reduce)
				# (there is no blockstriding) 3 results for 2 compilers
				combine3 ${gcc_funcs} ${clang_funcs} ${func} ${flag}
				;;
			*)
				# compare 5 results for 2 compilers
				combine5 ${gcc_funcs} ${clang_funcs} ${func} ${flag}
				;;
		esac
   done
done

echo combined plots are under comparison/

