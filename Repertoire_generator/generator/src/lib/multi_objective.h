/*
Parts of the later source code belongs to the PaGMO library ditributed as
free software; you can redistribute it and/or modify
it under the terms of either:
  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.
*/

#include "../NEAT/organism.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <ostream>
#include <vector>
#include <limits>

bool pareto_dominance(const std::shared_ptr<NEAT::Organism> org1,
                      const std::shared_ptr<NEAT::Organism> org2) {
    return ((org1->dNovelty_score >= org2->dNovelty_score &&
             org1->dLocal_competition >= org2->dLocal_competition) &&
            (org1->dNovelty_score > org2->dNovelty_score ||
             org1->dLocal_competition > org2->dLocal_competition));
}

auto compute_pareto_front(const std::vector<std::shared_ptr<NEAT::Organism>>& pop) {
    std::vector<std::vector<int>> non_dom_fronts(1);
    std::vector<std::vector<int>> dom_list(pop.size());
    std::vector<int> dom_count(pop.size());
    std::vector<int> front_rank(pop.size());

    for (unsigned int i = 0; i < pop.size(); i++) {
        dom_list[i].clear();
        dom_count[i] = 0;
        for (unsigned int j = 0; j < i; j++) {
            if (pareto_dominance(pop[i], pop[j])) {
                dom_list[i].push_back(j);
                dom_count[j]++;
            } else if (pareto_dominance(pop[j], pop[i])) {
                dom_list[j].push_back(i);
                dom_count[i]++;
            }
        }
    }

    for (unsigned int i = 0; i < pop.size(); i++) {
        if (dom_count[i] == 0) {
            front_rank[i] = 0;
            non_dom_fronts[0].push_back(i);
        }
    }

    auto dom_count_copy(dom_count);
    auto current_front = non_dom_fronts[0];
    std::vector<std::vector<int>>::size_type front_counter(0);
    while (current_front.size() != 0) {
        std::vector<int> next_front;
        for (unsigned int p = 0; p < current_front.size(); p++) {
            for (unsigned int q = 0; q < dom_list[current_front[p]].size(); q++) {
                dom_count_copy[dom_list[current_front[p]][q]]--;
                if (dom_count_copy[dom_list[current_front[p]][q]] == 0) {
                    front_rank[dom_list[current_front[p]][q]] = front_counter + 1;
                    next_front.push_back(dom_list[current_front[p]][q]);
                }
            }
        }
        front_counter++;
        current_front = next_front;
        if (current_front.size() != 0) {
            non_dom_fronts.push_back(current_front);
        }
    }

    return std::make_tuple(std::move(non_dom_fronts), std::move(dom_list), std::move(dom_count),
                           std::move(front_rank));
}

std::vector<double> crowding_distance_2d(
    const std::vector<int>& non_dom_front, const std::vector<std::shared_ptr<NEAT::Organism>>& pop,
    const std::vector<std::function<double(const std::shared_ptr<NEAT::Organism>&)>> list_get_fct) {
    auto N = non_dom_front.size(); // contains list of indexes of 1 front
    std::vector<int> indexes(N);
    std::iota(indexes.begin(), indexes.end(), 0);
    std::vector<double> retval(N, 0);

    for (auto getfct : list_get_fct) {
        std::sort(indexes.begin(), indexes.end(),
                  [&non_dom_front, &pop, &getfct](int idx1, int idx2) {
                      return getfct(pop[non_dom_front[idx1]]) > getfct(pop[non_dom_front[idx2]]);
                  });
        retval[indexes[0]] = std::numeric_limits<double>::infinity();
        retval[indexes[N - 1]] = std::numeric_limits<double>::infinity();
        double df =
            getfct(pop[non_dom_front[indexes[0]]]) - getfct(pop[non_dom_front[indexes[N - 1]]]);

        for (unsigned int j = 1; j < N - 1; j++) {
            retval[indexes[j]] += (getfct(pop[non_dom_front[indexes[j - 1]]]) -
                                   getfct(pop[non_dom_front[indexes[j + 1]]])) /
                                  df;
        }
    
        /* for (unsigned int j = 0; j < N ; j++) { */
        /*     std::cout << " j:" << j << " i:"<< indexes[j]<< " r:" <<  retval[indexes[j]] << std::endl; */
        /* } */
        /* std::cout << std::endl; */
    }
    return retval;
}
