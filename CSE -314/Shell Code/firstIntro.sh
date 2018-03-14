echo "Hello World"
pwd -> print present directory
cd ~ -> takes you to home directory
. -> current directory
.. -> parent directory
linux files and directories begin with "."
ls -a -> "-a" denotes all
ls -S -> sorts the files
ls -R -> show the whole subtree from the current directory
ls -l -> elaborates the permissions and informations in the directory

vim
i -> insert
esc -> escape
:w -> write to disk
less -> viewer
cat -> another viewer that concatenates but does not write unless ">"
cat -n -> add line number
head -15 c.txt | tail -5 -> pipelining reads 11-15 lines of c.txt

#! /bin/bash

echo Hello World!
#ls -l
x=`expr 1 + 2`
echo $x
read y
if [ $x \> $y ];then
	echo less
elif [ $x = $y ];then
	echo equal
else
	echo greater
fi

case "$y" in
yes | y | Yes | YES ) echo "Yes";;
n* | N* ) echo "No";;
* ) echo "else";;
esac

echo $#
for i in {2,3,6,8}
do
	sum=`expr $sum + $i`
	echo $i
done
echo $sum

for((i=0;i<10;i++))
do
	echo $i
done
#for i in `ls`
#do
#	echo $i
#done

my_func() {
	echo $#
	sum=0
	for i in $*
	do
		sum=`expr $sum + $i`
	done
	return $sum

}
my_func 2 3 7 8
echo $?

my_fact() {
	if [ $* = 1 ]; then
	return 1
	else
	x=`expr $1 - 1`
	my_fact $x
	y=`expr $1 \* $?`
fi
return $y
}
my_fact 5
echo $?

cd ~/Desktop/
for oldname in *.epub
do
	newname=${oldname%.epub}.pdf
	echo $newname
	mv $oldname $newname
done
