#! /bin/bash
for f in *.txt
do
	mv "$f" "`basename "$f" .txt`.c"
done
