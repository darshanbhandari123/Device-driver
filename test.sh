sudo insmod kernel_module/npheap.ko
sudo chmod 777 /dev/npheap
./benchmark/benchmark 256 8192 4
cat *.log > trace
sort -n -k 3 trace > sorted_trace
./benchmark/validate 256 8192 < sorted_trace
rm -f *.log
sudo rmmod npheap