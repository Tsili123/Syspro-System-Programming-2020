#!/bin/bash

IFS=$'\n' read -d '' -r -a diseases < /home/users/sdi1700170/syspro2/diseasesFile
array[0]="ENTER"
array[1]="EXIT"
size=${#array[@]}
size2=${#diseases[@]} 
mapfile names < ./names.txt
mapfile surnames < ./surnames.txt


filename=$2
while read line; do
# reading each line
	mkdir -p "./$3/$line"
	#echo $line
	for ((i=1; i<=($4/2); i++)); do 
		a=$((($RANDOM%31)+1))
		b=$((($RANDOM%13)+1))
		h=$(( ($RANDOM%21)+2000))
		#touch "./$3/$line/$a-$b-$h"
		k=$((($RANDOM%31)+1))
		l=$((($RANDOM%13)+1))
		m=$(($h+2))
		for ((j=1; j<=$5; j++)); do 
			d=$(($RANDOM%200000))
			c=$(( ($RANDOM%121)+1900))
			index=$(($RANDOM % $size))
			#e= ${array[$index]}
			index2=$(($RANDOM % $size2))
			f=${diseases[${index2}]}
			g=$((2020-$c))
			nameIndex=$RANDOM%$((${#names[@]}-1))
			surnameIndex=$RANDOM%$((${#surnames[@]}-1))
			name="$(echo -e "${names[$nameIndex]}" | tr -d '[:space:]')"
			surname="$(echo -e "${surnames[$surnameIndex]}" | tr -d '[:space:]')"
			printf -v x '%4d-%02d-%02d' $h $b $a
			#if [ $j == 4 ]
			#then
			#	echo "$d ${array[1]} $name $surname $f $g" >> "./$3/$line/$x"
			#else
				echo "$d ${array[0]} $name $surname $f $g" >> "./$3/$line/$x"
			#fi
			#index=$((($index+1) % $size))
			printf -v x '%4d-%02d-%02d' $m $l $k
			echo "$d ${array[1]} $name $surname $f $g" >> "./$3/$line/$x"
		done
	done
	#ls "./$3/$line" | sort -t '-' -k 3.1,3.4 -k 2.1,2.2 -k 1.1,1.2 
done < $filename
