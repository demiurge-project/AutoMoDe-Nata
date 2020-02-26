#!/usr/bin/python2

from __future__ import print_function
import argparse
from subprocess import *
import os
from os import listdir
from os.path import isfile, join
import re
import time
###############################################################################
# This script launches several runs in parallel in a SGE Cluster, and
# each run is parallelized using MPI.  Execute without parameters to see usage.
###############################################################################

########################### CONFIGURATION POINTS ##############################

QUEUE = 'short'
#MACHINE = 'opteron2216' #rack 1
#MACHINE = 'xeon5410' #rack 2
MACHINE = 'opteron6128' #rack 3
#MACHINE = 'opteron6272' #rack 4
#MACHINE = 'xeon2680' #rack 5

EVALUATION_CONTEXT = 'DesignContext'

###############################################################################

p = argparse.ArgumentParser(description='')
#p.add_argument('-i', '--instances', help="the intance or the folder containing the instances (.argos) for evaluation", type=str, required=True)
p.add_argument('-c', '--controllers', help="the absolute path to the parent folder in which all controllers are located", type=str, required=True)
p.add_argument('-n', '--expname', help='the name of the experiment', type=str, required=True)
p.add_argument('-d', '--resultdirectory', help='the directory where the result file should be saved', type=str, required=True)


JOB_SCRIPT = """#!/bin/bash
#$ -N %(jobname)s
#$ -l %(machine)s
#$ -l %(queue)s
#$ -m a
#      b     Mail is sent at the beginning of the job.
#      e     Mail is sent at the end of the job.
#      a     Mail is sent when the job is aborted or rescheduled.
#      s     Mail is sent when the job is suspended.
#$ -o %(execdir)s/argos3-auto.stdout
#$ -e %(execdir)s/argos3-auto.stderr
#$ -cwd
#$ -pe mpi %(nbjob)s
#$ -binding linear:256
export PATH
export LD_LIBRARY_PATH=/lustre/home/fpagnozzi/gcc71/lib64/:/home/aligot/argos3-dist/lib/argos3:/opt/gridengine/lib/lx-amd64:/opt/openmpi/lib
USERNAME=`whoami`
COMMAND=%(command)s
cd %(execdir)s
echo "-> %(command)s"
eval %(command)s
if [ $? -eq 0 ]
then
  echo "Success!"
  exit 0
else
  echo "Fail!"
  exit 1
fi
"""

def remove_ext(files):
    return [os.path.splitext(f)[0] for f in files]

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
     return [ atoi(c) for c in re.split('(.)(\d+)', text) ]

def keep_argos_instances(files):
    return [ f for f in files if f.endswith('.argos') ]

def submit_automode_job(args, instance, controllers, method):
     experiment_name = "%s-%s" % (args.expname, method)
     execdir = "ExecutionDir/" + experiment_name
     if not os.path.exists(execdir):
          os.makedirs(execdir)

     data = {"jobname": experiment_name,
             "execdir": execdir,
             "nbjob": 1,
             "machine": MACHINE,
             "queue": QUEUE,
             "command": "/home/aligot/AutoMoDe-Harlequin/evaluation/evaluate_chocolate.py -m %s -n %s -d %s -c %s -r %s -e %s" % (instance, args.expname, method, controllers, args.resultdirectory, EVALUATION_CONTEXT),
             }

     p = Popen("qsub -v PATH", shell=True, stdin=PIPE, stdout=PIPE, close_fds=True)
     (child_stdout, child_stdin) = (p.stdout, p.stdin)
     script = JOB_SCRIPT % data
     print(data.get("command"))
     child_stdin.write(script)
     child_stdin.close()
     print('Job sended')
     print(child_stdout.read())


def create_automode_jobs(args, method, mission_file):
    index_controller = 0
    cs_file = [f for f in listdir(join(args.controllers, method)) if isfile(join(args.controllers, method, f))]
    submit_automode_job(args, mission_file, join(args.controllers, method, cs_file[0]), method)

def submit_neat_job(args, instance, genomes_dir, method):
     experiment_name = "%s-%s" % (args.expname, method)
     execdir = "ExecutionDir/" + experiment_name
     if not os.path.exists(execdir):
          os.makedirs(execdir)

     data = {"jobname": experiment_name,
             "execdir": execdir,
             "nbjob": 1,
             "machine": MACHINE,
             "queue": QUEUE,
             "command": "/home/aligot/NeatvsEvopaper/evaluation/evaluate_neat.py -m %s -n %s -d %s -g %s -r %s -e %s" % (instance, args.expname, method, genomes_dir, args.resultdirectory, EVALUATION_CONTEXT),
             }
     p = Popen("qsub -v PATH", shell=True, stdin=PIPE, stdout=PIPE, close_fds=True)
     (child_stdout, child_stdin) = (p.stdout, p.stdin)
     script = JOB_SCRIPT % data
     print(data.get("command"))
     child_stdin.write(script)
     child_stdin.close()
     print('Job sended')
     print(child_stdout.read())


def create_neat_jobs(args, method, mission_file):
    genomes_dir = join(args.controllers, method)
    if (os.path.isdir(genomes_dir)):
        submit_neat_job(args, mission_file, genomes_dir, method)
    else:
        print("Error: unknown genomes location " + genomes_dir)


########
# Main #
########

if __name__ == "__main__":
    args = p.parse_args()

    config_files = [f for f in listdir(args.controllers) if f.endswith('.argos')]
    print(config_files)
    list_methods = ["Chocolate", "EvoStick", "Harlequin"]
    for method in list_methods:
        if ((method == "Chocolate")):
            mission_file = [f for f in config_files if f.startswith('chocolate_')][0]
            create_automode_jobs(args, method, join(args.controllers, mission_file))
        elif ((method == "Harlequin")):
            mission_file = [f for f in config_files if f.startswith('harlequin_')][0]
            create_harlequin_jobs(args, method, join(args.controllers, mission_file))
        else:
            mission_file = [f for f in config_files if f.startswith('evostick_')][0]
            create_neat_jobs(args, method, join(args.controllers, mission_file))
