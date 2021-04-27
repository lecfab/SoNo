#!/bin/bash
# parameter: type "./run.sh remove" if modified datasets should be removed to avoid heavy disk usage (1Tb otherwise)

datasets=(
    net_amazon
    net_LJ
    # net_orkut
)
dataset_name=(
    amazon
    LJ
    orkut
)

orders=(original rand degDesc degAsc dfs bfs slashburn ldg gorder) #
dirn=$((0 + $RANDOM % 10000))
dirr=../results/t${dirn}
# python_draw=../results/draw-orders-comparison.py


mkdir $dirr
echo "Results will be found in $dirr "
cat triangles.sh > $dirr/triangles_backup.sh
# cat $python_draw > $dirr/draw_backup.py

for dataset in ${!datasets[*]}
do
    echo "################## ${dataset_name[$dataset]} ##################"
	for order in ${orders[*]}
	do
        tee=${dirr}/tee-${dataset_name[$dataset]}-${order}

        echo "    --------- ${order} ---------"
        input=../datasets/${datasets[$dataset]}.txt
        echo "    ${input}"
        echo
        if [ $order == "original" ]; then
            output=../datasets/${datasets[$dataset]}.txt
        else
            output=../datasets/${datasets[$dataset]}-${order}.txt
    		if [ $1 == "remove" ] && [ -f "$output" ]; then
    			echo "    Edgelist already exists."
    		else
                rank=ord.tmp-${order}.txt
                cmd_ord="./ord $input $order  -o $rank"
                echo "    $cmd_ord"
                $cmd_ord | tee $tee-ord.txt
                echo
                cmd_rank="./rankedges $input $rank $output"
                echo "    $cmd_rank"
                $cmd_rank | tee $tee-rankedges.txt

                # rm $rank
    		fi
        fi
        echo

        results=${dirr}/time-${dataset_name[$dataset]}-${order}.txt
        if [ -f "$results" ]; then
            echo "    Results already exist."
            cat $results
        else
            cmd_benchmark="./alg $output -a triangles -o $results"
            echo "    $cmd_benchmark"
            $cmd_benchmark | tee $tee-benchmark.txt
            echo
        fi

		#cat $file
        # if [ $1 == "remove" ] && [ $order != "original" ]; then
        #     echo "    Removing edgelist $output"
        #     rm $output
        # fi
        echo
	done
    # break # only the smallest dataset

    echo

done

# echo "Creating figures for orders comparison"
# python3 $python_draw $dirn
