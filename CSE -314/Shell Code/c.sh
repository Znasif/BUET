#! /bin/bash

if [ $# -gt 0 ];then
	for i in $*
	do 
	if [ -f "$i" ];then
		chmod ugo-x $i
		echo "execution removed"
	else 
		echo "Not Executed"
		continue
	fi

	done
else
	
	echo "usage"
fi
