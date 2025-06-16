#!/usr/bin/env bash
# import profiling input parameters
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}}")" ; pwd -P )
cd "$parent_path"

input_dir=$1
output_dir=$2

for file in ${input_dir}/*; do
    filename=$(basename "$file")
    python3 ./convert.py --ppm_file ${file} --jpg_file ${output_dir}/${filename%.ppm}.jpg;
done
