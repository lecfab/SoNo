#!/bin/bash
# runtime of triangle counting for different orders

# ----------------------------------------
# ----- parameters, options, backups -----
# ----------------------------------------
datasets=(
    ../datasets/net_amazon.txt
    ../../danisch/DATA/SNAP/Amazon/net.txt
    ../../danisch/DATA/SNAP/LiveJournal/net.txt
    ../../danisch/DATA/SNAP/Wikipedia/net.txt
    ../../danisch/DATA/SNAP/Orkut/net.txt
    ../../danisch/DATA/Friendster/net.txt
)
dataset_name=(
    amazon
    LJ
    wiki
    orkut
    friendster
)
orders=(original rand core rcm degAsc dfs bfs slashburn gorder trianglesdpp trianglesdpm)
reverse=(0       0    1    0   0      0   0   0         0      0            0)
algos=(triangles)

remove=1 # should modified datasets be removed to avoid heavy disk usage? (90Gb if remove=1, 1Tb otherwise)
repeat=5
dirn=$((0 + $RANDOM % 10000))
dirr=../results/t${dirn}

mkdir $dirr
echo "Results will be found in $dirr "
cat run-triangles.sh > $dirr/backup-run.sh

# ----------------------------------------
# ------- main loop of experiments -------
# ----------------------------------------

# ----------- for each dataset -----------
for dataset in ${!datasets[*]}
do
    input=${datasets[$dataset]}
    if ! [ -f $input ]; then
        echo "Warning: dataset ${dataset_name[$dataset]} does not exist."
        continue
    fi

    echo "################## ${dataset_name[$dataset]} ##################"


    # ------------ for each order ------------
    for order_id in ${!orders[*]}; do
        order=${orders[$order_id]}
        tee=${dirr}/tee-${dataset_name[$dataset]}-${order}
        perf=${dirr}/perf-${dataset_name[$dataset]}-${order}

        echo "    --------- ${order} ---------"
        echo "    ${input}"
        echo
        # ------- compute and apply new ordering -------
        if [ $order == "original" ]; then
            output=${datasets[$dataset]}
        else
            output=../datasets/net_${dataset_name[$dataset]}-${order}.txt

    		if [ -f "$output" ]; then
    			echo "    Edgelist already exists."
    		else
                rank=ord.tmp-${order}.txt
                cmd_ord="./ord $input $order -u -o $rank"
                echo "    $cmd_ord"
                $cmd_ord | tee $tee-ord.txt
                echo
                if [ ${reverse[$order_id]} == "0" ]; then
                    cmd_rank="./rankedges $input $rank $output -u"
                else
                    cmd_rank="./rankedges $input $rank $output -ur" # reversed order
                fi
                echo "    $cmd_rank"
                $cmd_rank | tee $tee-rankedges.txt

                rm $rank
    		fi
        fi
        echo

        # ----- for each algorithm, measure runtime -----
        for algo in ${algos[*]}
    	do
            results=${dirr}/time-${dataset_name[$dataset]}-${order}-${algo}.txt
            if [ -f "$results" ]; then
                echo "    Results already exist."
                cat $results
            else
                cmd_benchmark="./alg $output -u -a $algo -o $results -l $repeat"
                echo "    $cmd_benchmark"
                $cmd_benchmark | tee $tee-alg-${algo}.txt
                echo
            fi
            sleep 1
            echo
        done

        # ------ remove edgelist if asked ------
        if [ $remove == 1 ] && [ $order != "original" ]; then
            echo "    Removing edgelist $output"
            rm $output
        fi
        echo
	done
    echo
done
