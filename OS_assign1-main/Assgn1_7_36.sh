mkdir -p $2&&touch $2/{a..z}.txt
for i in {a..z}
do
grep -hi "^$i" $1/*.txt>$2/$i.txt 
sort $2/$i.txt -o $2/$i.txt
done