#include "experiments.h"
#include "../lib/multi_objective.h"
#include "../lib/vector_tools.h"
#include <algorithm>
#include <iostream>
#include <locale>
#include <memory>
#include <stdio.h>
/**
 * Function that launches NEAT: load all the useful parameters for NEAT, create the first population
 *based on the starter genome, call the experiment and evaluation defined by the user, and create
 *the next generations.
 * @params: neatParams - file containing the useful parameters for NEAT.
 *	    startGenes - file containing the structure of the starter genome.
 *	    (*fctExperiment)(Network&) - function defined by the user that launches the experiment
 *on a network, and return the evaluation on this last one.
 * */
void launchNEAT(const char* neatParams, const char* startGenes, double (*fctExperiment)(Network&),
                unsigned int seed) {

    // Useful variables
    Population* pop = NULL;
    Genome* startGenome = NULL;
    char curword[31];
    int id;

    if (seed == 0) {
        // Random Setup: Seed the random-number generator with current time.
        srand((unsigned)time(NULL));
    } else {
        srand(seed);
    }

    // Load all the useful parameters for NEAT
    NEAT::load_neat_params(neatParams, true);

    // Create the starter genome
    std::ifstream iFile(startGenes, std::ios::in);
    iFile >> curword;
    iFile >> id;
    startGenome = new Genome(id, iFile);
    iFile.close();

    // For each run
    for (int run = 0; run < NEAT::num_runs; run++) {

        // Set the new population based on the starter genome.
        pop = new Population(startGenome, NEAT::pop_size);
        // For each generation
        for (unsigned int g = 1; g <= NEAT::num_gens; g++) {
            printf("\n Generation %d\n", g);

            // Evaluate each organism on a test
            id = 1;
            for (std::vector<Organism*>::iterator org = (pop->organisms).begin();
                 org != (pop->organisms).end(); ++org, id++) {

                printf("Organism: %d\n", id);

                // Launch the experiment with the specified network passed in argument, and evaluate
                // this last one
                (*org)->fitness = (*fctExperiment)(*((*org)->net));

                printf("Fitness: %f\n", (*org)->fitness);
            }

            // Write the result in a file.
            if (NEAT::print_every != 0 && (g % NEAT::print_every) == 0) {
                sprintf(curword, "gen/gen_%d_%d", run, g);
                pop->print_to_file_by_species(curword);
                // pop->print_species_champs_tofiles();
            }

            // Reproduction and creation of the next generation is done by NEAT
            pop->epoch(g);
        }

        // Todo: Post-Evaluation

        // Delete the population
        delete pop;
    }

    // Delete the genome
    delete startGenome;
}

/**
 * Function that launches NEAT: load all the useful parameters for NEAT, create the first population
 *based on the starter genome, call the experiment and evaluation defined by the user, and create
 *the next generations.
 * @params: neatParams - file containing the useful parameters for NEAT.
 *	    startGenes - file containing the structure of the starter genome.
 *	    (*fctExperiment)(Population&) - function defined by the user that launches the
 *experiment on a whole population. The evaluation of each organism can be done in parallel. This
 *parallelism is left to the user to define it.
 * */
void launchNEAT(const char* neatParams, const char* startGenes,
                void (*fctExperiment)(Population&, unsigned int), unsigned int seed) {

    // Useful variables
    Population* pop = NULL;
    Genome* startGenome = NULL;
    char curword[31];
    int id;

    if (seed == 0) {
        // Random Setup: Seed the random-number generator with current time.
        srand((unsigned)time(NULL));
    } else {
        srand(seed);
    }

    // Load all the useful parameters for NEAT
    NEAT::load_neat_params(neatParams, true);

    // Create the starter genome
    std::ifstream iFile(startGenes, std::ios::in);
    iFile >> curword;
    iFile >> id;
    startGenome = new Genome(id, iFile);
    iFile.close();

    // For each run
    for (int run = 1; run <= NEAT::num_runs; run++) {
        // Set the new population based on the starter genome.
        pop = new Population(startGenome, NEAT::pop_size);

        // For each generation
        for (unsigned int g = 1; g <= NEAT::num_gens; g++) {

            // Launch the experiment <NEAT::num_runs_per_gen> times with the specified population
            // passed in argument, and evaluate each organism in this last one.
            (*fctExperiment)(*pop, NEAT::num_runs_per_gen);

            // Write the result in a file.
            if (NEAT::print_every != 0 && (g % NEAT::print_every) == 0) {
                sprintf(curword, "gen/gen_%d_%d", run, g);
                pop->print_to_file_by_species(curword);
                sprintf(curword, "gen/gen_%d_%d_champ", run, g);
                pop->print_champ_to_file(curword);
                // pop->print_species_champs_tofiles();
            }

            // Reproduction and creation of the next generation is done by NEAT
            pop->epoch(g);
        }
        // Post-Evaluation of the last population
        if (NEAT::num_runs_post_eval != 0) {
            std::cout << "\nPost-Evaluation" << std::endl;
            (*fctExperiment)(*pop, NEAT::num_runs_post_eval);
        }

        // Write the result in a file.
        sprintf(curword, "gen/gen_last_%d", run);
        pop->print_to_file_by_species(curword);
        sprintf(curword, "gen/gen_last_%d_champ", run);
        pop->print_champ_to_file(curword);
        // pop->print_species_champs_tofiles();

        // Delete the population
        delete pop;
    }

    // Delete the genome
    delete startGenome;
}

/**
 * Function that launches NEAT: load all the useful parameters for NEAT, create the first population
 *based on the starter genome, call the experiment and evaluation defined by the user, and create
 *the next generations.
 * @params: neatParams - file containing the useful parameters for NEAT.
 *	    startGenes - file containing the structure of the starter genome.
 *	    (*fctExperiment)(Population&) - function defined by the user that launches the
 *experiment on a whole population. The evaluation of each organism can be done in parallel. This
 *parallelism is left to the user to define it.
 * */
void launchNEATRepertoires(const char* neatParams, const char* repertoireParamsFile,
                           const char* startGenes, void (*fctExperiment)(Population&, unsigned int),
                           unsigned int seed) {

    // Useful variables
    Population* pop = NULL;
    Genome* startGenome = NULL;
    char curword[31];
    int id;

    if (seed == 0) {
        // Random Setup: Seed the random-number generator with current time.
        srand((unsigned)time(NULL));
    } else {
        srand(seed);
    }

    // Load all the useful parameters for NEAT
    NEAT::load_neat_params(neatParams, true);
    // Load all the useful parameters for the repertoires
    repertoireParams params;
    load_repertoire_params(repertoireParamsFile, params);

    // Create the starter genome
    std::ifstream iFile(startGenes, std::ios::in);
    iFile >> curword;
    iFile >> id;
    startGenome = new Genome(id, iFile);
    iFile.close();

    // Init the archive and container (repertoire)
    std::vector<std::shared_ptr<Organism>> vArchive;   // The organisms in the archive
    std::vector<std::shared_ptr<Organism>> vContainer; // The organisms in the container

    // For each run
    for (int run = 1; run <= NEAT::num_runs; run++) {
        // Set the new population based on the starter genome.
        pop = new Population(startGenome, NEAT::pop_size);

        // For each generation
        for (unsigned int g = 1; g <= NEAT::num_gens; g++) {
            // Launch the experiment <NEAT::num_runs_per_gen> times with the specified population
            // passed in argument, and evaluate each organism in this last one.
            (*fctExperiment)(*pop, NEAT::num_runs_per_gen);
            // make deepcopy of pop organisms for ease prupose
            std::vector<std::shared_ptr<Organism>> vOrganisms(pop->organisms.size());
            std::transform(pop->organisms.begin(), pop->organisms.end(), vOrganisms.begin(),
                           [](auto org) { return std::make_shared<Organism>(*org); });

            unsigned int nb_neighbors = params.nb_neighbors;
            for (auto org : vOrganisms) {
                // find nb_neighbors nearest neighbors of org in archive and pop
                // those will be shared_ptr:
                auto neighbors_pop = K_nearest_neighbors(org, vOrganisms, nb_neighbors);
                auto neighbors_archive = K_nearest_neighbors(org, vArchive, nb_neighbors);
                std::vector<std::shared_ptr<Organism>> neighbors;
                std::set_union(neighbors_pop.begin(), neighbors_pop.end(),
                               neighbors_archive.begin(), neighbors_archive.end(),
                               std::back_inserter(neighbors));

                /* std::cout << org<< " 0: " << neighbors[0] << " l: "<<
                 * neighbors[neighbors.size()-1] << std::endl; */

                if (neighbors.size() > nb_neighbors)
                    neighbors.resize(nb_neighbors);
                // compute novelty score N(i) of org is: mean behaviour distance to the k-nearest
                //                                    neighbours (Algorithm 1, step 14),
                //                                    encompassing both the individuals from the
                //                                    current population and individuals from an
                //                                    archive
                // compute local competition score LC(i) of org is: corresponds to the number of
                //                                            individuals in the same
                //                                            k-nearest neighbours that are
                //                                            outperformed by the individual
                //                                            currently under evaluation, with
                //                                            respect to a provided quality metric
                org->dNovelty_score = 0; // reset scores in case of old organism
                org->dLocal_competition = 0;

                for (auto neighbor : neighbors) {
                    org->dNovelty_score +=
                        vectors_distance(neighbor->vMed_behavior_char, org->vMed_behavior_char);
                    if (org->dQuality_score > neighbor->dQuality_score)
                        org->dLocal_competition += 1;
                }
                org->dNovelty_score /= nb_neighbors;

                /* std::cout << "dNovelty_score " << org->dNovelty_score << std::endl; */
                // find 2 nearest neighbors of org in container
                // those will be shared_ptr
                auto neighbors_container = K_nearest_neighbors(org, vContainer, 2);
                auto dist_threshold = params.dist_threshold;
                auto dist_constant = params.dist_constant;
                if (neighbors_container.size() > 0) {
                    auto dist = vectors_distance(neighbors_container[0]->vMed_behavior_char,
                                                 org->vMed_behavior_char);
                    // if very different add to container
                    /* std::cout << " diff: " << dist; */
                    if (dist > dist_threshold) {
                        vContainer.push_back(org);
                        /* std::cout << " push back very diff: "<< dist << std::endl; */
                    } else if (neighbors_container.size() > 1) {
                        // else if better than one , replace
                        auto dist = vectors_distance(neighbors_container[1]->vMed_behavior_char,
                                                     org->vMed_behavior_char);
                        if (dist > dist_threshold * (1 - dist_constant) and
                            org->dQuality_score > neighbors_container[0]->dQuality_score) {
                            auto best_one = std::find(vContainer.begin(), vContainer.end(),
                                                      neighbors_container[0]);
                            vContainer.erase(best_one);
                            vContainer.push_back(org);
                            /* std::cout << "push back better one" << std::endl; */
                        }
                    }
                } else {
                    vContainer.push_back(org); // push first one
                    /* std::cout << "push back first one" << std::endl; */
                }
            }

            // update fitness with pareto score of all orgs
            auto pareto_lists = compute_pareto_front(vOrganisms);
            std::vector<std::function<double(const std::shared_ptr<NEAT::Organism>&)>> list_fct = {
                [](std::shared_ptr<NEAT::Organism> org) { return (double)org->dNovelty_score; },
                [](std::shared_ptr<NEAT::Organism> org) {
                    return (double)org->dLocal_competition;
                }};

            /* std::cout << "id,crowd,paretofront,fitness,LC,nov,qual" << std::endl; */
            auto index = 0;
            for (auto front : std::get<0>(pareto_lists)) {
                auto retval = crowding_distance_2d(front, vOrganisms, list_fct);
                std::vector<int> indexes(retval.size());
                std::iota(indexes.begin(), indexes.end(), 0);

                std::sort(indexes.begin(), indexes.end(),
                          [&retval](int idx1, int idx2) { return retval[idx1] > retval[idx2]; });

                for (auto id : indexes) {
                    /* vOrganisms[front[id]]->fitness = vOrganisms.size() - index; */
                    pop->organisms[front[id]]->fitness = vOrganisms.size() - index; 
                    index++;

                    /* std::cout << front[id] << ", vorgf: " << vOrganisms[front[id]]->fitness << "," << std::endl; */
                    /* std::cout << id << ", popff: " << pop->organisms[front[id]]->fitness << "," << std::endl; */
                    /* std::cout << id << "," << retval[id] << "," */ 
                    /*           << std::get<3>(pareto_lists)[front[id]] */
                    /*           << vOrganisms[front[id]]->dLocal_competition << "," */
                    /*           << vOrganisms[front[id]]->dNovelty_score << "," */
                    /*           << vOrganisms[front[id]]->dQuality_score << std::endl; */
                }
            }

            unsigned int gen_growth = params.gen_growth;
            unsigned int max_archive_size = params.max_archive_size;
            // Check archive size and remove org if needed
            if (vArchive.size() > max_archive_size - gen_growth) {
                std::random_shuffle(vArchive.begin(), vArchive.end());
                // Remove gen_growth random orgs
                vArchive.erase(vArchive.begin(), vArchive.begin() + gen_growth);
            }

            // Put gen_growth new orgs in archive
            std::random_shuffle(vOrganisms.begin(), vOrganisms.end());
            if (vOrganisms.size() > gen_growth)
                vOrganisms.resize(gen_growth);
            for (auto org : vOrganisms) {
                vArchive.push_back(org);
            }

            // Write the result in a file.
            if (NEAT::print_every != 0 && (g % NEAT::print_every) == 0) {
                sprintf(curword, "gen/gen_%d_%d", run, g);
                pop->print_to_file_by_species(curword);
                sprintf(curword, "gen/gen_%d_%d_champ", run, g);
                pop->print_champ_to_file(curword);
                // pop->print_species_champs_tofiles();
            }

            // Reproduction and creation of the next generation is done by NEAT
            pop->epoch(g);
        }
        std::cout << "Going for post-eval.." << std::endl;
        // Post-Evaluation of the last population
        if (NEAT::num_runs_post_eval != 0) {
            std::cout << "\nPost-Evaluation" << std::endl;
            (*fctExperiment)(*pop, NEAT::num_runs_post_eval);
        }

        // Write the result in a file.
        sprintf(curword, "gen/gen_last_%d", run);
        pop->print_to_file_by_species(curword);
        sprintf(curword, "gen/gen_last_%d_champ", run);
        pop->print_champ_to_file(curword);
        // pop->print_species_champs_tofiles();

        // TODO save archive (?)
        // save repertoire
        sprintf(curword, "gen/repertoire_%d", run);
        std::ofstream oFile(curword);
        for (unsigned int i = 0; i < vContainer.size(); i++) {
            char tempbuf[1024];
            sprintf(tempbuf, "/* Organism #%d Quality: %f Time: %d Id: %d */\n/* ",
                    (vContainer[i]->gnome)->genome_id, vContainer[i]->dQuality_score,
                    vContainer[i]->time_alive, i);
            oFile << tempbuf;
            std::copy(vContainer[i]->vMed_behavior_char.begin(),
                      vContainer[i]->vMed_behavior_char.end(),
                      std::ostream_iterator<double>(oFile, " "));
            sprintf(tempbuf, " */\n");
            oFile << tempbuf;
            vContainer[i]->gnome->print_to_file(oFile);
            sprintf(tempbuf, "\n\n");
            oFile << tempbuf;
        }
        oFile.close();
        // Delete the population
        delete pop;
        vArchive.clear();   // clear organisms in the archive
        vContainer.clear(); // clear organisms in the container
    }

    // Delete the genome
    delete startGenome;
}

bool load_repertoire_params(const char* filename, repertoireParams& params) {

    std::ifstream paramFile(filename);

    if (!paramFile) {
        return false;
    }
    char curword[128];

    // **********LOAD IN PARAMETERS*************** //
    printf("Repertoire loading params from: %s \n", filename);

    paramFile >> curword;
    paramFile >> params.nb_neighbors;

    paramFile >> curword;
    paramFile >> params.dist_threshold;

    paramFile >> curword;
    paramFile >> params.dist_constant;

    paramFile >> curword;
    paramFile >> params.gen_growth;

    paramFile >> curword;
    paramFile >> params.max_archive_size;

    paramFile.close();

    std::cout << "nb_neighbors: " << params.nb_neighbors << std::endl;
    std::cout << "dist_threshold: " << params.dist_threshold << std::endl;
    std::cout << "dist_constant: " << params.dist_constant << std::endl;
    std::cout << "gen_growth: " << params.gen_growth << std::endl;
    std::cout << "max_archive_size: " << params.max_archive_size << std::endl;

    return true;
}
