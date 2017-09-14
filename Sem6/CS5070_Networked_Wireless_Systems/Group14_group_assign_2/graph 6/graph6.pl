set terminal png size 640,480
set output "graph6.png"
set title "Aggregate System Throughputs for different Handover Algorithms"
set xlabel "Handover Algorithms"
set ylabel "Average Aggregate System throughput(bps)"
set style data histogram
set style fill solid border -1
set style histogram cluster gap 1
set boxwidth 0.9
plot for [COL=2:4] 'graph6.txt' using COL:xticlabels(1) title columnheader