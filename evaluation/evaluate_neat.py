#!/usr/bin/python2

from __future__ import print_function
import sys, getopt
from subprocess import *
import os
from os import listdir
from os.path import isfile, join
import re
import time

###############################################################################
# This script launches ...
###############################################################################

########################### CONFIGURATION POINTS ##############################

AUTOMODE_BIN_DIR = "/home/aligot/NEATvsEvo/NEAT/bin"

# p = argparse.ArgumentParser(description='runs automode and collects the results')
# p.add_argument('-m', '--mission', help='the mission (.argos) to launch', type=str, required=True)
# p.add_argument('-c', '--controllerfile', help='the file containing the fsm', type=str, required=True)
# p.add_argument('-i', '--indexcontroller', help='the index of the fsm in the controller file', type=int, required=True)
# p.add_argument('-r', '--repetitions', help='the number of time the experiment has to be repeated', type=int, required=True)
# p.add_argument('-n', '--expname', help='the name of the experiment', type=str, required=True)

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
     return [ atoi(c) for c in re.split('(.)(\d+)', text) ]

def run_neat(mission_file, genomes_dir, expname, method, resultdir, evaluation_context):
    experiment_name = "%s-%s" % (expname, method)
    print(experiment_name)
    output_file = open("/home/aligot/NeatvsEvopaper/evaluation/" + resultdir + "/" + experiment_name + ".txt", 'w')
    seeds = open("/home/aligot/NeatvsEvopaper/evaluation/seeds.txt", 'r').readlines()

    genome_index = 0
    genomes = os.listdir(genomes_dir)
    genomes.sort(key=natural_keys)
    for genome in genomes:
        genome_file = join(genomes_dir, genome)
        seed = int(seeds[genome_index])
        output_file.write("\"" + method + "\",\"" + expname + "\",\"" + evaluation_context + "\",\"" + str(seed) + "\",")
        full_cmd = AUTOMODE_BIN_DIR + '/NEAT-launch' + ' -s ' + str(seed) + ' -c ' + mission_file + ' -g ' + genome_file
        print(full_cmd)
        p = Popen(full_cmd.split(" "), stdin=PIPE, stdout=PIPE, stderr=PIPE)
        output, err = p.communicate(b"input data that is passed to subprocess' stdin")
        print(output)
        print(err)
        vec_output = output.split("\n")
        score = vec_output[len(vec_output)-2].split(" ")[1]
        output_file.write(str(score) + '\t')
        output_file.write('\n')
        genome_index += 1
    output_file.close()

if __name__ == "__main__":
    mission_file = ""
    genomes_dir = ""
    expname = ""
    resultdir = ""
    method = ""
    evaluation_context = ""
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:g:n:d:r:e:", ["mission=", "genomesdir=", "expname=", "designmethod=", "resultdir=", "evaluation="])
    except getopt.GetoptError:
        print("evaluate_automode.py -m <mission> -g <genome file> -d <design method> -n <name of exp> -r <result dir> -e <evaluation context>")
        sys.exit(2)
    for opt, arg in opts:
        if opt == "-h":
            print("evaluate_automode.py -m <mission> -g <genome file> -d <design method>  -n <name of exp> -r <result dir> -e <evaluation context>")
            sys.exit()
        elif opt in ("-m", "--mission"):
            mission_file = arg
        elif opt in ("-g", "--genomesdir"):
            genomes_dir = arg
        elif opt in ("-n", "--expname"):
            expname = arg
        elif opt in ("-d", "--designmethod"):
            method = arg
        elif opt in ("-r", "--resultdir"):
            resultdir = arg
        elif opt in ("-e", "--evaluation"):
            evaluation_context = arg

    run_neat(mission_file, genomes_dir, expname, method, resultdir, evaluation_context)
