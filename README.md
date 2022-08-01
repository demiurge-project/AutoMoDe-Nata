# AutoMoDe-Nata and Repertoire generator

## Description

We present on this repository the code used in the paper `AutoMoDe-Nata`
to generate a repertoire of swarm robotics behaviors (here, for the e-puck robot),
and the `AutoMoDe` flavor `Nata` that uses this repertoire to automatically assemble
finite-state machines. Note that the repertoire can be automatically generated
or not, this makes this code compatible with `AutoMoDe-Arlequin`.

## Package content
```
├── `Nata` this folder contains all sources corresponding to `AutoMoDe-Nata`
│   ├── `bin` this empty folder will contain the executable files.
│   ├── `env_rep.path` example file that contains the full path for the repertoire to use
│   ├── `gen` folder that contains the  repertoire of `Arlequin`
│   ├── `missions` missions xml files for argos3 simulator
│   ├── `optimization` scenarios to use with irace to generate the robot control software
│   └── `src` source code of `Nata`
├── `Repertoire_generator`
│   ├── `generator`
│   │   ├── `bin` this empty folder will contain the executable files.
│   │   ├── `experiments` argos3 xml configuration files for repertoire creation
│   ├── `gen` this folder can store genomes
│   │   ├── `misc` this folder contains various script used to launch experiments
│   │   ├── `optimization` scenarios to use with irace to generate the robot control software
│   │   ├── `params` this folder contains the hyperparameter files for the generation of the repertoires
│   │   ├── `src` the source code of the repertoire generator
│   │   └── `startgen` the starting genomes for the repertoire generation
│   └── `results`
│       ├── `designRep.toml`
│       └── `repertoire`
└── `scripts`
```

## Dependencies:
- [ARGoS3](https://github.com/ilpincy/argos3) (3.0.0-beta48)
- [argos3-epuck](https://github.com/demiurge-project/argos3-epuck) (v48)
- [experiments-loop-functions](https://github.com/demiurge-project/experiments-loop-functions) (repertoire)
- [demiurge-epuck-dao](https://github.com/demiurge-project/demiurge-epuck-dao) (master)

## How to use

### Quick install

First install all system dependencies:
```
sudo apt install cmake libfreeimage-dev libfreeimageplus-dev \
  qt5-default freeglut3-dev libxi-dev libxmu-dev \
  doxygen graphviz graphviz-dev asciidoc git \
  libboost-all-dev libeigen3-dev openmpi-bin openmpi-common libtbb-dev python3-pip
```

```
pip3 install cppyy
```

We provide an automatic script (tested under Ubuntu 18.04) to install all software and their dependencies.

In this folder navigate to scripts and install all code repositories:
```
cd scripts
./bundle_argos3_epuck.sh
```

This should install the whole argos3 distribution and librairies in /home/<youruser>/argos-dist/. After the installation, if everything went smoothly, setup your $PATH for argos3:

```
./home/<youruser>/argos3-dist/bin/setup_argos
```

This command should be run after each login on the computer, you can add it to your `.bashrc` to make it automatic.

This command will also create the `config.py` file that contains informations about the path where ARGoS will find for the librairies and tools used for this experiment.

### Creating the `env.rep` repertoire identification file

In order for the simulator to locate the repertoire in use, a file named
`env.rep` should be created in the current working directory where the 
simulator is launched.

This file should contain the absolute path to the repertoire to be used.

For example for Nata:
`<path_to_this_repo>/Repertoire_generator/results/repertoire/rep7_500g_d7-0/`

### Using an existing repertoire and an already generated finite-state machine.

After installation, (given config.py is correctly setup),
if you wish to run an individual control software, it is done using the launchArgos.py script:

```
usage: launchArgos.py [-h] [--config CONFIG] [--gen GEN] [--fsm-config FSM_CONFIG] [--seed SEED]

Launch argos with options and automatic path resolving

optional arguments:
  -h, --help                                show this help message and exit
  --config CONFIG, -c CONFIG                argos configuration file (.argos)
  --gen GEN, -g GEN                         genome file (if launching Neuro evolution method)
  --fsm-config FSM_CONFIG, -f FSM_CONFIG    fsm descrption (if launching automode method)
  --seed SEED, -s SEED
```



Example for  `Nata`:

```
python3 scripts/launchArgos.py -s 98128 -c Nata/missions/foraging.argos --fsm-config " "
```

Example for `Evostick`:
```
python3 scripts/launchArgos.py -s 98128 -c Nata/missions/foraging.argos -g " " 
```

Examples of genomes and PFSM that can be used with those scripts can be found
in the `Controllers.zip` file associated with the research paper and published
as [IEEE Dataport](https://ieee-dataport.org/documents/towards-automatic-design-automatic-methods-design-robot-swarms-results-and-controllers)


### Automatic design of finite-state machine
Using an existing repertoire to generate finite-state machines for a mission.

For running the optimization process, you need to have R with the irace package.
To install R and irace:
```
sudo apt install r-base
wget https://github.com/MLopez-Ibanez/irace/archive/master.zip
R CMD INSTALL master.zip
```

After installation, set your $PATH for using irace:
```
export IRACE_HOME=~/R/irace/ # Path given by system.file(package="irace")
export PATH=${IRACE_HOME}/bin/:$PATH
```

Navigate to the optimization folder of `Nata` and launch irace:

```
cd AutoMoDe-Nata/Nata/optimization
irace --exec-dir=execdir --parallel 10 --max-experiments 50000 --scenario scenario.txt
```

This will launch a design phase in the current folder.
For the algorithm to be able to locate the repertoire of behaviors to be used,
you should create a `env.rep` file containing the path of the repertoire.

### Creation of a repertoire
How to create a new repertoire of behaviors to be used to generate finite-state
machines with the previous procedure.

/!\ This process requires a CPU cluster to be run in a acceptable amount of
time. The scripts provided require modification according to the specific
cluster computing engine that is used.

```
python misc/paralell_run_mpi_local_rep.py -d repertoire -j rep_500g -n 48 -e ../src/experiments/repertoireTrainingCluster.argos -p ../src/params/EvostickRepertoireParams500.ne -s ../src/startgen/evostickstartgenesRM11 -pr ../src/params/repertoiresParams7.rep -r 1
```

## Manuel installation
All the software prerequisite have to be installed before hand.

Clone this repository

Navigate to the repository and build the package AutoMoDe-Nata
```cd Nata
mkdir build
cd build
cmake .. && make
```

Then, from the root of the repository, navigate to the repertoire generator and
build it
```cd Repertoire_generator/generator
mkdir build
cd build
cmake .. && make
```


