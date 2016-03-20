#!/bin/bash

files="./temp/*"

for filepath in ${files}
do
	python convertRGB565.py $filepath
done

