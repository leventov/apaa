#!/bin/bash
BIR=GBigInt
for i in {0..100}
do
	g++ -O3 -D NOP_COUNT=repeat$i  -c $BIR.cpp	-Wno-pointer-arith
	g++ -O3  -o main BI_c.o $BIR.o main.cpp -lrt
	./main
done
