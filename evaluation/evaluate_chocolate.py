#!/usr/bin/python2

from __future__ import print_function
import sys, getopt
from subprocess import *
import os
import re
import time

###############################################################################
# This script launches ...
###############################################################################

########################### CONFIGURATION POINTS ##############################

AUTOMODE_BIN_DIR = "/home/aligot/AutoMoDe-private/bin"

def run_automode(mission_file, controller_file, expname, method, resultdir, evaluation_context):
    experiment_name = "%s-%s" % (expname, method)
    output_file = open("/home/aligot/NeatvsEvopaper/evaluation/" + resultdir + "/" + experiment_name + ".txt", 'w')
    seeds = open("/home/aligot/NeatvsEvopaper/evaluation/seeds.txt", 'r').readlines()
    controllers = open(controller_file, 'r').readlines()
    controller_index = 0
    for controller in controllers:
        seed = int(seeds[controller_index])
        output_file.write("\"" + method + "\",\"" + expname + "\",\"" + evaluation_context + "\",\"" + str(seed) + "\",")
        full_cmd = AUTOMODE_BIN_DIR + '/automode_main' + ' -s ' + str(seed) + ' -c ' + mission_file + ' --fsm-config ' + controller
        print(full_cmd)
        p = Popen(full_cmd.split(" "), stdin=PIPE, stdout=PIPE, stderr=PIPE)
        output, err = p.communicate(b"input data that is passed to subprocess' stdin")
        print(output)
        print(err)
        vec_output = output.split("\n")
        score = vec_output[len(vec_output)-2].split(" ")[1]
        output_file.write(str(score) + '\t')
        output_file.write('\n')
        controller_index += 1
    output_file.close()

if __name__ == "__main__":
    mission_file = ""
    controller_file = ""
    expname = ""
    resultdir = ""
    method = ""
    evaluation_context = ""
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:c:n:d:r:e:", ["mission=", "controllerfile=", "expname=", "method=", "resultdir=", "evaluation="])
    except getopt.GetoptError:
        print("evaluate_automode.py -m <mission> -c <controller file> -n <name of exp> -m <design method> -r <result dir> -e <evaluation context>")
        sys.exit(2)
    for opt, arg in opts:
        if opt == "-h":
            print("evaluate_automode.py -m <mission> -c <controller file> -n <name of exp> -m <design method> -r <result dir> -e <evaluation context>")
            sys.exit()
        elif opt in ("-m", "--mission"):
            mission_file = arg
        elif opt in ("-c", "--controllerfile"):
            controller_file = arg
        elif opt in ("-n", "--expname"):
            expname = arg
        elif opt in ("-d", "--designmethod"):
            method = arg
        elif opt in ("-r", "--resultdir"):
            resultdir = arg
        elif opt in ("-e", "--evaluation"):
            evaluation_context = arg

    run_automode(mission_file, controller_file, expname, method, resultdir, evaluation_context)
