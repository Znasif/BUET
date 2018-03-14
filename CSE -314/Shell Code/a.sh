#! /bin/bash

if [ $# -gt 0 ];then
	for i in $*
	do 
	if [ -f "$i" ] || [ -d "$i" ];then
		
		echo "can not be created"
	else 
		echo "$i" | grep -q [/.]
		if [ "$?" = 0 ];then
			touch "$i"
		else 
			mkdir "$i"
		fi
		continue
	fi

	done
else
	
	echo "usage"
fi
