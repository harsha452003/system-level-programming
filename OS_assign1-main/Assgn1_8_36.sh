if [ ! -f "main.csv" ]
then
    touch main.csv
    chmod +w main.csv
    chmod +x main.csv
    echo "Date,Category,Amount,Name" >> main.csv
fi

args=($@)
num=$#

if [ $num -ge 4 ]
then
    for (( i=1; i<5; i++ ))
    do
        res=$(( num-i ))
        case $i in
            1)
                Name=${args[$res]};;
            2)
                Amount=${args[$res]};;
            3)
                Category=${args[$res]};;
            4)
                Date=${args[$res]};;
        esac
    done
    if [[ $Date =~ ^[0-9]{2}-[0-9]{2}-[0-9]{2}$ ]]
    then
        echo "$Date,$Category,$Amount,$Name" >> main.csv
        echo "Inserted $Date,$Category,$Amount,$Name in main.csv"
        tail -n +2 main.csv | sort -t, -k1.7n,1.8n -k1.4n,1.5n -k1.1n,1.2n -o temp.csv
        echo "Date,Category,Amount,Name" > main.csv
        cat temp.csv >> main.csv
    fi

    if [ -f "temp.csv" ]
    then
        rm temp.csv
    fi
fi

while getopts ":c:n:s:h" flag
do
    case $flag in
        c)
            category=$OPTARG 
            grep -v "^Date" main.csv | awk -F, '{if($2 == "'"$category"'") {tot+=$3;found=1}} END {if(found == 1)print "Total amount spent on '"$category"': " tot; else print "Invalid Category"}';;
        n)
            name=$OPTARG 
            grep -v "^Date" main.csv | awk -F, '{if($4 == "'"$name"'") {tot+=$3;found=1}} END {if(found == 1)print "Total amount spent by '"$name"': " tot; else print "Invalid Name"}';;
        s)
            column=$OPTARG
            case $column in
                "Date")
                     tail -n +2 main.csv | sort -t, -k1.7n,1.8n -k1.4n,1.5n -k1.1n,1.2n -o temp.csv
                    echo "Date,Category,Amount,Name" > main.csv
                    cat temp.csv >> main.csv;;
                "Category")
                    tail -n +2 main.csv | sort -t, -k2,2 -k1.7n,1.8n -k1.4n,1.5n -k1.1n,1.2n -o temp.csv
                    echo "Date,Category,Amount,Name" > main.csv
                    cat temp.csv >> main.csv;;
                "Amount")
                     tail -n +2 main.csv | sort -t, -k3,3 -n -k1.7n,1.8n -k1.4n,1.5n -k1.1n,1.2n -o temp.csv
                    echo "Date,Category,Amount,Name" > main.csv
                    cat temp.csv >> main.csv;;
                "Name")
                    tail -n +2 main.csv | sort -t, -k4,4 -k1.7n,1.8n -k1.4n,1.5n -k1.1n,1.2n -o temp.csv
                    echo "Date,Category,Amount,Name" > main.csv
                    cat temp.csv >> main.csv;;
                *)
                    echo "Error: Invalid column name"
                    exit 1;;
            esac;;
        h)
            green='\033[0;36m'
            clear='\033[0m'
            echo -e "${green}NAME${clear}"
            echo "      Assgn1_8_36.sh - lists and provides all expenditure in the tour"
            echo ""
            echo -e "${green}SYNOPSIS${clear}"
            echo "      Assgn1_8_36.sh [-c 'Category'] [-n 'name] [-s 'column'] Record...."
            echo "      Assgn1_8_36.sh -h"
            echo ""
            echo -e "${green}DESCRIPTION${clear}"
            echo "      Inserts the Record(s) into the main.csv file, if file does not exist it creates and insert"
            echo "      Flags are optional but record is mandatory with column format {Date Category Amount Name}"
            echo ""
            echo "      -c 'Category'"
            echo "                accepts a category and prints the amount of money spend in that category"
            echo ""
            echo "      -n 'name'"
            echo "                accepts a name and print the amount spent by that person"
            echo ""
            echo "      -s 'column'"
            echo "                sorts the main.csv file by column name if column is present else invalid"
            echo ""
            echo "      -h"
            echo "                displays this help prompt and exit"
            exit 1;;
        \?)
            echo "-$OPTARG : Invalid flag"
            exit 1;;
        :)
            echo "-$OPTARG flag requires an argument"
            exit 1;;
    esac

    if [ -f "temp.csv" ]
    then
        rm temp.csv
    fi
done 