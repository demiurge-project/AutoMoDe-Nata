#!/bin/bash
controls=10
jobs=("Aggregation" "Foraging" "Shelter")
jobs_dir=("aggregation" "foraging" "shelter")
index=(2) #(0 1 2)
for i in ${index[@]}
do
	./${jobs_dir[$i]}/parallel-irace-mpi $controls ${jobs[$i]}_200k --parallel 24
done

