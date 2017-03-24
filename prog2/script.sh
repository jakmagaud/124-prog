#!/bin/bash
for i in `seq 1 128`;
do
	./strassen $i 1024 mat4.txt
done
