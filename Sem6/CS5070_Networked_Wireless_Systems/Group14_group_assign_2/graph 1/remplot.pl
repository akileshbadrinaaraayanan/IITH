set terminal png size 640,480
set output "Rem.png"
set title "Radio Environment Map"
set xlabel "X"
set ylabel "Y"
set cblabel "SINR (dB)"
unset key
plot "rem.out" using ($1):($2):(10*log10($4)) with image
