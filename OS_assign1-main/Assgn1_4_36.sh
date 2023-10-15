while read line
do 
    echo "$line" | awk -v word="$2" '{split($0,a," ");for(i in a){if(a[i]==word){exit 0}};exit 1}'
    if([[ $? -eq 0 ]])then
        echo "${line,,}" | sed 's/[a-z][^a-z\n]*[a-z]\?/\u\0/g'
    else echo "$line"
    fi
done < $1