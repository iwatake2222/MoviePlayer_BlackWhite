#!/bin/bash

files="./temp/*"

for filepath in ${files}
do
	python convertBlack.py $filepath
done

