#!/bin/bash
declare -A x y
while read a b
do  x[$b]=$((${x[$b]}+1))&&y[$a]=$((${y[$a]}+1))
done < $1
for a in "${!x[@]}"
do
echo "$a ${x[$a]}"
done | sort -k1 | sort -k2r >output9.txt
for b in "${!y[@]}"
do
if([ ${y[$b]} -gt 1 ])then
echo "$b"
else ans=$(($ans+1))
fi 
done>> "output9.txt"
echo $ans >> "output9.txt"
