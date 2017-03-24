#!/bin/bash
for i in `seq 1 128`;
do
	./strassen $i 256 mat.txt
done
