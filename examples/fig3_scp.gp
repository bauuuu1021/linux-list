reset
set ylabel 'time(sec)'
set xlabel 'size'
set key left top
set style fill solid
set title 'random vs sequential(size 50000)'
set term png enhanced font 'Verdana,10'
set output 'fig3.png'

plot [:][:20]'size_50000.txt' \
   using 2:xtic(1) with histogram title 'insert(random)', \
'' using 3:xtic(1) with histogram title 'insert(seq)' , \
'' using 4:xtic(1) with histogram title 'quick(random)' , \
'' using 5:xtic(1) with histogram title 'quick(seq)' , \
'' using 6:xtic(1) with histogram title 'merge(random)',  \
'' using 7:xtic(1) with histogram title 'merge(seq)' 