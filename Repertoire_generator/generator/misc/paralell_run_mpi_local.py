#!/usr/bin/python2

from __future__ import print_function
import argparse
from subprocess import *
import os
import re
import time
import random
###############################################################################
# This script launches several runs in parallel in a SGE Cluster, and
# each run is parallelized using MPI.  Execute without parameters to see usage.
###############################################################################

########################### CONFIGURATION POINTS ##############################

NEAT_DIR = "/home/khasselmann/Arlequin/src"

QUEUE='long' #or 'short'
#MACHINE='opteron2216' #rack 1
# MACHINE='xeon5410' #rack 2
#MACHINE='opteron6128' #rack 3
#MACHINE='opteron6272' #rack 4
MACHINE='xeon2680' #rack 5
# MACHINE='xeon6138' #rack 6

###############################################################################

p = argparse.ArgumentParser(description='runs a run multiple times in paralell using mpi, python paralell_run_mpi.py')
p.add_argument('-d', '--dir', help='the execdir', required=True)
p.add_argument('-j', '--jobname', help='the name of the job', required=True)
p.add_argument('-n', '--nbcores', help='number of parallel processes', type=int, required=True)
p.add_argument('-e', '--exp', help='the experiment xml', required=True)
p.add_argument('-p', '--params', help='the file for the parameters of the evolution', required=True)
# p.add_argument('-pr', '--paramsRep', help='the file for the parameters for the repertoires', required=True)
p.add_argument('-s', '--startgenes', help='the file for the starting genome', required=True)
p.add_argument('-r', '--runs', help='number of runs to do (number of sequential jobs) (default=1)', type=int, default=1)
p.add_argument('-m', '--machine', help='the machine to run to (default TBD in script)', default=MACHINE)
p.add_argument('-q', '--queue', help='the queue to run to (default TBD in script)', default=QUEUE)


def run_neat(args,run):
    data = {"jobname": "neatevo-%i-%i" % (os.getpid(),run),
            "machine": args.machine,
            "queue": args.queue,
            "execdir": (os.path.abspath(args.dir)+ ("/%s-%i" % (args.jobname, run))),
            "nbcores": args.nbcores,
            "neatdir": NEAT_DIR,
            "experiment": os.path.abspath(args.exp),
            "params": os.path.abspath(args.params),
            # "paramsRep": os.path.abspath(args.paramsRep),
            "startgenes": os.path.abspath(args.startgenes),
            "seed": random.randrange(2, 2147483647), # 2147483647 is the maximum of a signed int
    }
    script = """#!/bin/bash
#$ -N %(jobname)s
#$ -l %(machine)s
#$ -l %(queue)s
#$ -m base
#      b     Mail is sent at the beginning of the job.
#      e     Mail is sent at the end of the job.
#      a     Mail is sent when the job is aborted or rescheduled.
#      s     Mail is sent when the job is suspended.
#$ -o %(execdir)s/argos3-neat.stdout
#$ -e %(execdir)s/argos3-neat.stderr
#$ -cwd
#$ -pe mpich_fu %(nbcores)s
#$ -binding linear:256

export OMPI_MCA_plm_rsh_disable_qrsh=1
export PATH
export LD_LIBRARY_PATH=/lustre/home/fpagnozzi/gcc91/lib64/:/home/khasselmann/argos3-dist/lib/argos3:$LD_LIBRARY_PATH

USERNAME=`whoami`
NEATDIR=%(neatdir)s
COMMAND="$NEATDIR/bin/NEAT-evolution %(experiment)s %(params)s %(startgenes)s %(nbcores)s $NEATDIR/bin/scheduler %(seed)s"

cd %(execdir)s
echo "$COMMAND"
eval $COMMAND
if [ $? -eq 0 ]
then
  echo "Success!"
  exit 0
else
  echo "Fail!"
  exit 1
fi
"""
    p = Popen("qsub -v PATH", shell=True, stdin=PIPE, stdout=PIPE, close_fds=True)
    (child_stdout, child_stdin) = (p.stdout, p.stdin)
    child_stdin.write(script % data)
    #print(script % data)
    child_stdin.close()
    print('Job sended')
    print(child_stdout.read())

if __name__ == "__main__":
    args = p.parse_args()

    for run in range(args.runs):
        os.makedirs(os.path.abspath(args.dir) + ("/%s-%i" % (args.jobname, run)))
        os.makedirs(os.path.abspath(args.dir) + ("/%s-%i" % (args.jobname, run)) + "/gen")
        run_neat(args,run)
        time.sleep(1)
