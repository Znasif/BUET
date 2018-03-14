sum=0
while [ 1 ]
	do
	read a
	if [ $a -lt 0 ];then
		continue
	elif [ $a -gt 0 ];then
		sum=`expr $sum + $a`
	else 
		break
	fi
done
echo $sum
