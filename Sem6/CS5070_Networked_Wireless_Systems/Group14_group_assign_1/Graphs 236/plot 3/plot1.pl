
set terminal png size 640,480
set output "plot1.png"
set title "Throughput CDF plot for different schedulers at Speed 0 m/s for full buffer scenario"
set xlabel "Throughput(Mbps)"
set ylabel "CDF"
#plot "plot11.dat" using 1:2 title "pfff", "plot12.dat" using 1:2 lt 3 title "rrff"
set style data linespoints

set multiplot



# common y-range
#set yrange [0:1.3e+9]

plot "plot11.dat" using 1:2  title "PF",\
	 "plot12.dat" using 1:2   title "RR",\
	 "plot13.dat" using 1:2   title "MT",\
	 "plot14.dat" using 1:2   title "BATS"
unset multiplot


