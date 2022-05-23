#!/usr/bin/python3
import cppyy
from cppyy import addressof, bind_object
import pathlib
import tempfile
import sys
import xml.etree.ElementTree as ET
import config
import argparse


def load_argos():
    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/core/simulator/simulator.h"
        )
    )
    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/core/simulator/space/space.h"
        )
    )
    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/core/simulator/loop_functions.h"
        )
    )
    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/core/utility/plugins/dynamic_loading.h"
        )
    )
    cppyy.load_library(
        str(pathlib.Path(config.ARGOS_PREFIX) / "lib/argos3/libargos3core_simulator.so")
    )

    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/plugins/robots/e-puck/simulator/epuck_entity.h"
        )
    )

    cppyy.include(
        str(
            pathlib.Path(config.ARGOS_PREFIX)
            / "include/argos3/demiurge/loop-functions/CoreLoopFunctions.h"
        )
    )
    from cppyy.gbl import argos

    mak = argos.CSimulator.GetInstance()
    arr = argos.CDynamicLoading.LoadAllLibraries()

    return mak, arr


def main(args):
    mak, arr = load_argos()

    tree = ET.parse(args.config)
    root = tree.getroot()
    controllers = root.find("controllers")
    if controllers:
        for controller in controllers:
            id = controller.get('id')
            controller.set("library", config.CONTROLLERS[id])
            path = pathlib.Path(controller.get("library"))
            print(path)
            if args.gen and id == "nn_rm_1dot1" :
                print("Adding gen file")
                controller.find('params').set('genome_file', args.gen)
            elif args.fsm_config and id == "automode":
                print('Adding fsm config')
                controller.find('params').set('fsm-config', args.fsm_config)

    loops = root.find("loop_functions")
    if loops:
        findloop = pathlib.Path(config.LOOP_PREFIX).glob("**/*.so")
        old_loop_path = pathlib.Path(loops.get("library"))
        looplist = [ loop for loop in findloop if loop.name == old_loop_path.name ]
        if len(looplist) == 0:
            print('Could not find the loopfunc in the prefix')
            raise
        looppath = looplist[0]
        if len(looplist) > 1:
            print('multiple possible loopfunc file found: {}'.format(looplist))
        print('Using loopfunc: {}'.format(looppath))
        loops.set('library', str(looppath.absolute()))
        # print(looppath)

    tmpfile = tempfile.NamedTemporaryFile()
    tree.write(tmpfile.name)

    mak.SetExperimentFileName(tmpfile.name)
    mak.SetRandomSeed(int(args.seed))
    mak.LoadExperiment()

    # print(type(mak), type(args.gen))
    cSpace = mak.GetSpace()
    mak.Execute()

    loopfunc = mak.GetLoopFunctions()
    from cppyy.gbl import CoreLoopFunctions

    loopfunc = bind_object(addressof(loopfunc), CoreLoopFunctions)
    score = loopfunc.GetObjectiveFunction()
    print("Score {}".format(score))

    mak.Destroy()
    tmpfile.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Launch argos with options and automatic path resolving")
    parser.add_argument('--config', '-c', help="argos configuration file (.argos)")
    parser.add_argument('--gen', '-g', help="genome file (if launching Neuro evolution method)")
    parser.add_argument('--fsm-config', '-f', help="fsm descrption (if launching automode method)")
    parser.add_argument('--seed', '-s', help="random seed to use")
    args = parser.parse_args()
    print(args)
    main(args)
