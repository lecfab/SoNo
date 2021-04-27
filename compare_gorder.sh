#!/bin/bash

dataset=../datasets/edgelist-flickr-2M-33M
dataset_name=flickr
repeat=10
declare -i loops=10
if (( $# > 0 )); then
    loops=$1
fi
declare -i loop=0

dirr=../results/gorder-${loops}-samos
mkdir $dirr
for loop in $(seq 0 $(($loops - 1))); do
    output_hao=$dirr/hao-${dataset_name}.${loop}.txt
    output_fab=$dirr/fab-${dataset_name}.${loop}.txt

    echo "----------------------"
    echo "--------- loop=${loop}"
    echo "----------------------"
    echo

    echo
    echo "    --------- Fab's Gorder ---------"
    cmd_fab="./benchmark ${dataset}-gorder.txt -A -o $output_fab -l $repeat"
    echo "    $cmd_fab"
    $cmd_fab

    echo "    --------- Hao's Gorder ---------"
    cmd_hao="./benchmark ${dataset}_Gorder.txt -A -o $output_hao -l $repeat"
    echo "    $cmd_hao"
    $cmd_hao
done
