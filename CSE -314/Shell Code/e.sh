#! /bin/bash
read li
read p
for i in $*
do
	x=`expr 0 + $li`
	q=$(head -$x "$i"| tail -1) 
	echo "$q" | grep -q "$p"
	if [ $? = 0 ];then
		rm "$i"
		echo "File Deleted"
	else
		echo "Not Found"

	fi
done
