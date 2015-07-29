#!/bin/bash

in_file=$(find . | grep -P "\.py|\.cpp|\.hpp")
for in_file in $in_file; do
	sed 's/^\(\t*\)\(  \)/\1\t/g' "$in_file" > "${in_file}xxx"
	mv "${in_file}xxx" "$in_file"
done
