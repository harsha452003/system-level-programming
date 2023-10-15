for file in $(find $1 -name '*.py' -print);do 

    echo
    echo Filename : $(basename $file)
    echo Filepath : $file
    echo
    echo Single line Comments,if any:
    echo
    f=0;
    c=1;
    while read line;do
    if [[ $line =~ ^\s*[\"][\"][\"].*$ ]]&&[ "$f" -eq "0" ]; then 
    f=1;
    elif [[ $line =~ [\"][\"][\"] ]]&&[ "$f" -eq "1" ];then 
    f=0;
    fi;
    if [[ $line =~ ^\s*[\'][\'][\'].*$ ]]&&[ "$f" -eq "0" ]; then 
    f=1;
    elif [[ $line =~ [\'][\'][\'] ]]&&[ "$f" -eq "1" ];then 
    f=0;
    fi;
    echo $line| grep -qhiE "(^|[^\"'])#.*"
    if [ $? -eq "0" ] && [ "$f" -eq "0" ]; then
    echo $c:$line >> temp.txt
    grep -hiE "(^|[^\"'])#.*" temp.txt | sed -e 's/^\([0-9]*\):\(.*\)#\(.*\)/\1:\3/' 
    rm temp.txt
    fi;
    ((c++));
    done < $file
    f=0
    c=1

    echo
    echo Mutliline comments,if any:
    echo
    while read line;do
    if [[ $line =~ ^\s*[\"][\"][\"].*[\"][\"][\"]$ ]]; then echo $c: 
    echo $line
    echo
    elif [[ $line =~ ^\s*[\"][\"][\"].*$ ]]&&[ "$f" -eq "0" ]; then 
    echo  $c: 
    echo $line
    f=1;
    elif [[ $line =~ [\"][\"][\"] ]]&&[ "$f" -eq "1" ];then 
            echo $line
            echo
            f=0;
    elif [ "$f" -eq "1" ];then echo $line;
    fi;
    ((c++));
    done < $file
    f=0
    c=1

    while read line;do
    if [[ $line =~ ^\s*[\'][\'][\'].*[\'][\'][\']$ ]]; then echo $c: 
    echo $line
    echo
    elif [[ $line =~ ^\s*[\'][\'][\'].*$ ]]&&[ "$f" -eq "0" ]; then 
    echo  $c: 
    echo $line
    f=1;
    elif [[ $line =~ [\'][\'][\'] ]]&&[ "$f" -eq "1" ];then 
            echo $line
            echo
            f=0;
    elif [ "$f" -eq "1" ];then echo $line;
    fi;
    ((c++));
    done < $file
    

done