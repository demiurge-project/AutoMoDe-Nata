#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <ostream>
#include <vector>

std::vector<double> vector_add(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> C(A.size());
    std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::plus<double>());
    return C;
}

std::vector<double> vector_div(const std::vector<double>& A, double scalar) {
    std::vector<double> C(A.size());
    std::transform(A.begin(), A.end(), C.begin(), [&scalar](auto c) { return c / scalar; });
    return C;
}

std::vector<double> geometric_median(const std::vector<std::vector<double>>& X,
                                     double distance(const std::vector<double>&,
                                                     const std::vector<double>&),
                                     int iterations = 200) {
    auto N = X.size();
    auto dim = X[0].size();
    if (N < 3)
        return X[0];
    /* std::cout << "Too small nb of elts"<< std::endl; */

    std::vector<double> geo_median(dim);
    // initial guess
    std::vector<std::vector<double>> A(2, vector_div(vector_add(X[0], X[1]), 2.0));

    for (int it = 0; it < iterations; it++) {
        std::vector<double> numerator(dim, 0);
        double denominator = 0;

        auto t = it % 2;

        for (unsigned int n = 0; n < N; n++) {
            double dist = distance(X[n], A[t]);

            if (dist != 0) {
                numerator = vector_add(numerator, vector_div(X[n], dist));
                denominator += 1.0 / dist;
            }
        }

        A[1 - t] = vector_div(numerator, denominator);
    }

    geo_median = A[iterations % 2];
    return geo_median;
}

double vectors_distance(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> auxiliary;

    std::transform(A.begin(), A.end(), B.begin(), std::back_inserter(auxiliary),
                   [](double element1, double element2) { return pow((element1 - element2), 2); });
    auxiliary.shrink_to_fit();

    return sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
}

template <typename LstPtr, typename Ptr>
LstPtr K_nearest_neighbors(const Ptr org, const LstPtr& list_orgs, unsigned int k) {
    LstPtr copy_list_orgs = list_orgs;
    std::sort(copy_list_orgs.begin(), copy_list_orgs.end(), [&org](auto a, auto b) {
        return vectors_distance(a->vMed_behavior_char, org->vMed_behavior_char) <
               vectors_distance(b->vMed_behavior_char, org->vMed_behavior_char);
    });
    if (copy_list_orgs.size() > 0 and
        vectors_distance(org->vMed_behavior_char, copy_list_orgs[0]->vMed_behavior_char) == 0) {
        copy_list_orgs.erase(copy_list_orgs.begin());
    }
    if (copy_list_orgs.size() > k)
        copy_list_orgs.resize(k);
    return copy_list_orgs;
}
