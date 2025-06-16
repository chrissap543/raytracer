#!/usr/bin/env bash
# import profiling input parameters
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}}")" ; pwd -P )
cd "$parent_path"
source ./profiling_input_parameters.sh
echo ${number_grid_list}
for dp in ${depths[@]}; do
    for sp in ${samples[@]}; do
        for wd in ${widths[@]}; do
            python3 ./filter_csv.py --infile $1/speedup.csv --outdir $1 --width ${wd} --samples ${sp} --grid ${number_grid_list}
        done
    done
done
