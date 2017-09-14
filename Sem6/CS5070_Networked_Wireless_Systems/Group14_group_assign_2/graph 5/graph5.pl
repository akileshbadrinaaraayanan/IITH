set terminal png size 640,480
set output "graph5.png"
set title "Effect of Cell Offset on no. of Handovers"
set xlabel "Cell Offset"
set ylabel "Avg. No. of Handovers for 20m/s, 50m/s, 100m/s"
set style data histogram
set style fill solid border -1
set style histogram cluster gap 1
set boxwidth 0.9
plot for [COL=2:4] 'graph5.txt' using COL:xticlabels(1) title columnheader