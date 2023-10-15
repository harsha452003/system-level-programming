# !/bin/bash
function gcd() {
    x=$1
    y=$2
    while [ $y -gt 0 ]
    do
        t=$x
        x=$y
        y=$((t%y))
    done
    echo $x
}
function lcm() {
    x=$1
    y=$2
    g=$(gcd $x $y)
    echo $((x*(y/g)))    
}
LCM=1
while read num 
do
temp=$num
reverse=
while [ $temp -ne 0 ]
do
    reverse=$reverse$((temp%10))
    temp=$((temp/10))
done
temp3=$(lcm $LCM $reverse) 
LCM=$temp3          
done < $1
echo $LCM
