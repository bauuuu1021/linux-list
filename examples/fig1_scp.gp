reset
set ylabel 'time(sec)'
set xlabel 'size'
set key left top
set title 'time comparison(random input)'
set term png enhanced font 'Verdana,10'
set output 'fig1.png'

plot [:][:20]'all_random.txt' \
   using 2:xtic(1) with linespoints linewidth 3 title 'insert', \
'' using 3:xtic(1) with linespoints linewidth 3 title 'quick' , \
'' using 4:xtic(1) with linespoints linewidth 3 title 'merge'  