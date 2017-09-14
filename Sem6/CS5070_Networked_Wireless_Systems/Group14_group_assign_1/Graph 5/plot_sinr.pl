set terminal png size 640,480 #(sets the image format and dimensions)
set output "graph5_sinr.png" #(sets the image name)

#set xdata time
#set timefmt "%S"
set xlabel "Time (s)"

set autoscale

set ylabel "SINR (dBm)"
#set format y "%s"

set title "SINR for diff scheduling algo at 5 m/s"
#set style line 5 lt rgb "cyan" lw 1 pt 2

set style data linespoints

#plot "sinr_PfFf.txt" using 1:6 with lines title "SINR PfPf(dbm)", \
 #    "sinr_FdMt.txt" using 1:6 title with lines "SINR FdMt(dbm)", \
  #   "sinr_RrFf.txt" using 1:6 title with lines "SINR RrFf(dbm)", \ 		
   #  "sinr_FdBet.txt" using 1:6 title with lines "SINR FdBet(dbm)" 	

plot "sinr_PfFf.txt" using 1:6 title "SINR PF(dbm)", \
     "sinr_FdMt.txt" using 1:6 title "SINR MT(dbm)", \
     "sinr_RrFf.txt"  using 1:6 title "SINR RR(dbm)", \
     "sinr_FdBet.txt"  using 1:6 title "SINR BATS(dbm)"
