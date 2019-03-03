reset
set ylabel 'time(sec)'
set xlabel 'size'
set key left top
set title 'quick sort (random vs sequenial)'
set term png enhanced font 'Verdana,10'
set output 'vs.png'

plot [:][:25]'vs.txt' \
   using 2:xtic(1) with linespoints linewidth 3 title 'random', \
'' using 3:xtic(1) with linespoints linewidth 3 title 'sequential' 