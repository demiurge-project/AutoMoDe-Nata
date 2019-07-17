#!/bin/bash
randseed=(10 20 30 40 50)
modules=("Exploration" "Stop") #("Exploration" "Stop" "Phototaxis" "AntiPhototaxis" "Attraction" "Repulsion")
rmodules=("Stop" "Exploration") #("Stop" "Exploration" "AntiPhototaxis" "Phototaxis" "Repulsion" "Attraction")
fsmconf=("0 --rwm0 5" "1") #("0 --rwm0 5" "1" "2" "3" "4 --att0 2.5" "5 --rep0 2.5")
index=(0 1) #(0 1 2 3 4 5)
for i in ${index[@]}
do
	for j in ${randseed[@]}
	do
		echo ${modules[$i]}"_seed"$j
		../../../bin/automode_main -c "test${modules[$i]}.argos"  --fsm-config --nstates 1 --s0 ${fsmconf[$i]} --seed $j
	done
done
for i in ${index[@]}
do
	for j in ${randseed[@]}
	do
		echo "Reverse_"${rmodules[$i]}"_seed"$j
		../../../bin/automode_main -c "testR${rmodules[$i]}.argos" --fsm-config --nstates 1 --s0 ${fsmconf[$i]} --seed $j
	done
done
