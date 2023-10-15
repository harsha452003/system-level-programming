#!/bin/bash
if [[ ! -e /output.txt ]]; then
    touch /output.txt
fi
truncate -s 0 output.txt

primes=()

limit=1000
declare -a sieve
for ((i=0; i<=limit; i++))
do
    sieve[i]=true
done
sieve[0]=false
sieve[1]=false
for ((i=2; i*i<=limit; i++))
do
    if [ ${sieve[i]} = true ]
    then
        for ((j=i*i; j<=limit; j+=i))
        do
            sieve[j]=false
        done
    fi
done

for ((i=2; i<=limit; i++))
do
    if [ ${sieve[i]} = true ]
    then
        primes+=($i)
    fi
done

while read x; do
a=$(bc <<< "scale=0; sqrt(($x))")
divisors=()
for ((i=0; i<${#primes[@]} ; i++))
do
    if [[ ${primes[$i]} -gt $a ]]
    then
    break
    fi
    if [[ $x%${primes[$i]} -eq 0 ]] 
    then
    #   echo ${primes[i]}
      divisors+=(${primes[i]})
    fi
done
if [[ ${#divisors[@]} -eq 0 ]]
then
  divisors+=($x)
fi  
echo "The divisors of $x are: ${divisors[@]}" | tee -a output.txt
done < input.txt

