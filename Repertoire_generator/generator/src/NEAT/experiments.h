#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include "neat.h"
#include "network.h"
#include "organism.h"
#include "population.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
//#include "genome.h"
//#include "species.h"

using namespace NEAT;

struct repertoireParams {
    unsigned int nb_neighbors;
    float dist_threshold;
    float dist_constant;
    unsigned int gen_growth;
    unsigned int max_archive_size;
};

void launchNEAT(const char* neatParams, const char* startGenes, double (*fctExperiment)(Network&),
                unsigned int seed = 0);
void launchNEAT(const char* neatParams, const char* startGenes,
                void (*fctExperiment)(Population&, unsigned int), unsigned int seed = 0);
void launchNEATRepertoires(const char* neatParams, const char* repertoireParamsFile,
                           const char* startGenes, void (*fctExperiment)(Population&, unsigned int),
                           unsigned int seed = 0);

bool load_repertoire_params(const char* filename, repertoireParams& params);

#endif
