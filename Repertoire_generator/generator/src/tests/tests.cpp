#include "../lib/multi_objective.h"
#include "../lib/vector_tools.h"

#include <iostream>
#include <vector>

int main() {
    std::cout << "Running tests" << std::endl;

    std::vector<double> a = {1, 0, 5};
    std::vector<double> b = {0, 2, 4};

    auto added = vector_add(a, b);
    for (auto i : added) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    auto div = vector_div(b, 2);
    for (auto j : div) {
        std::cout << j << ", ";
    }
    std::cout << std::endl;

    auto dist = vectors_distance(a, b);
    std::cout << dist << std::endl;

    //(2, 3), (8, 4), (12, -3), (0, 0), (0, 0)     -> (2.17, 1.29)
    std::vector<std::vector<double>> vecs = {{2, 3}, {8, 4}, {12, -3}, {0, 0}, {0, 0}};
    std::vector<double> Med = geometric_median(vecs, vectors_distance);

    for (auto j : Med) {
        std::cout << j << ", ";
    }
    std::cout << std::endl;

    class Org {
      public:
        std::vector<double> vMed_behavior_char;
    };
    std::vector<std::shared_ptr<Org>> vOrganisms;

    for (int i = 0; i < 5; i++) {
        auto org = std::make_shared<Org>();
        org->vMed_behavior_char = {(double)i, 0};
        vOrganisms.push_back(org);
    }

    std::cout << "Org:";
    for (auto j : vOrganisms) {
        for (auto i : j->vMed_behavior_char) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }

    auto org = std::make_shared<Org>();
    org->vMed_behavior_char = {1, 0.001};
    auto neighbors_pop = K_nearest_neighbors(org, vOrganisms, 2);

    std::cout << "Neighs:";
    for (auto j : neighbors_pop) {
        for (auto i : j->vMed_behavior_char) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
