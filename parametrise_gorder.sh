#!/bin/bash
#for k in $(seq 3 20);

# dataset=edgelist-epinion-75k-508k
# dataset_name=epinion
dataset=edgelist-flickr-2M-33M
dataset_name=flickr
algo=pr
declare -i loops=10
if (( $# > 0 )); then
    loops=$1
fi
declare -i loop=0
mkdir ../results/window/

for loop in $(seq 0 $(($loops - 1))); do
    echo "----------------------"
    echo "--------- loop=${loop}"
    echo "----------------------"

    window=1
    while (( $window < 2000000 )); do
        echo "    --------- Window=${window} ---------"
        input=../datasets/${dataset}.txt
        rank=ord.auto.txt
        output=../datasets/${dataset}-gorder${window}.txt

        if [ -f "$output" ]; then
    		echo "    File already exists."
    	else
            cmd_ord="./ord $input gorder -d -o $rank -n $window"
            echo "    $cmd_ord"
            $cmd_ord
            echo
            cmd_rank="./rankedges $input $rank $output"
            echo "    $cmd_rank"
            $cmd_rank

            rm $rank
        fi

    	file=../results/window/window-${dataset_name}-${algo}-${window}.${loop}.txt
    	# echo "    ${file}"

    	if [ -f "$file" ]; then
    		echo "    File already exists."
            cat $file
    	else
            cmd_benchmark="./benchmark $output -a $algo -o $file -l 10"
            echo "    $cmd_benchmark"
            $cmd_benchmark
            echo
    	fi

        echo

        window=$((window * 2))
    done
done
