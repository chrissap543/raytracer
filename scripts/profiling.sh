#!/bin/bash
# Input arguments: File to csv file, path to image and common name, file to bin
echo "Profiling the implementation of the ray tracer!"

#if [ -f $1 ]; then
#    echo "Profiling file exists."
#else
#    touch $1;
#    chmod 777 $1;
#    echo "Created profiling file."
#fi

source ../scripts/profiling_input_parameters.sh
count=0

for nrGd in ${number_grid[@]}; do
    #echo $nrGd
    for dp in ${depths[@]}; do
        #echo $dp
        for sp in ${samples[@]}; do
            #echo $sp
            for wd in ${widths[@]}; do
                echo
                echo RUN: ${count}
                echo "$wd" "$sp" "$dp" "$nrGd"
                "$3" "$1" "${wd}" "${sp}" "${dp}" "${nrGd}" > "$2_${wd}_${sp}_${dp}_${nrGd}.ppm"
                ((count++))
                #echo ${image_path}_${wd}_${sp}_${dp}_${nrGd}.ppm
            done
        done
    done
done
