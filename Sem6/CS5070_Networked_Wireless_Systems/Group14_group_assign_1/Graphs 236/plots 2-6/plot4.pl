 set terminal png size 640,480 
 set xlabel "Speed (5 m/s)"
 set ylabel "Average throughput (bps) "
set style line 1 lc rgb "red"
set style line 2 lc rgb "blue"
set style line 3 lc rgb "green"
set style line 4 lc rgb "yellow"
set yrange [1.00E+06 : 1.5E+06 ]

set output "output4.png"

set style fill solid
set boxwidth 0.5
set title ' Average throughput (bps) vs Speed(m/s) Half Buffer case '

plot "plot4.dat" every ::0::0 using 1:3:xtic(2) title 'PF' with boxes ls 1, \
     "plot4.dat" every ::1::1 using 1:3:xtic(2) with boxes title'RR' ls 2, \
     "plot4.dat" every ::2::2 using 1:3:xtic(2) with boxes title 'MT' ls 3, \
     "plot4.dat" every ::3::3 using 1:3:xtic(2) with boxes title 'BATS' ls 4
