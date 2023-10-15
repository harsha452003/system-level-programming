while read line
do 
    echo $line|grep -qif fruits.txt
    if [ $? -eq "1" ]&&[[ $line =~ ^[a-zA-Z]+[a-zA-Z0-9]*[0-9]+[a-zA-Z0-9]*$ ]]&&[[ $line =~ ^.{5,20}$ ]]
    then 
        echo "YES" >> validation_results.txt
        continue
    fi
    echo "NO" >> validation_results.txt
done < $1