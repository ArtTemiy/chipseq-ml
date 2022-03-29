# for results
mkdir sorted_data

for filename in ./raw_data/*.bed; do
    echo $(basename $filename)
    bedtools sort -i $filename > ./sorted_data/$(basename "$filename" .bed).sorted.bed
done
