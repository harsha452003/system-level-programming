#!/bin/bash
input_folder=$1
output_folder=$2
attributes=("$@")
attributes=("${attributes[@]:2}")
echo ${attributes[@]}
mkdir temp ans
for file in $input_folder/*.jsonl; do
    jq -n '[inputs]' <$file >temp/$(basename "$file" .jsonl).json
done

for file in temp/*.json; do
  filename=$(basename "$file" .json)
  header=""
  val=""
  for attribute in "${attributes[@]}"; do
    header="$header,$attribute"
    val="$val.$attribute,"
  done
  val=${val%?}
  header="${header:1}"
  jq -r ".[] | [$val] | @csv" "$file" | sed "1s/^/$header\n/" | tr -d '"' > "$output_folder/$filename.csv" 
done
echo "done"
rm -r temp
