//
// Created by abraham on 24/12/23.
//

#ifndef PRACTICAS_ANNEALING_HPP
#define PRACTICAS_ANNEALING_HPP

#include <random>
#include "vector"
#include "GrafoP.hpp"
#include "math.h"
#include <algorithm>

template<typename Z>
static double avgDistance(const GrafoP<Z>& costsGraph){

    double dst = 0.0;

    for (size_t i = 0; i < costsGraph.numVert()-1; ++i) {
        for (size_t j = i+1; j < costsGraph.numVert(); ++j) {
            dst += costsGraph[i][j];
        }
    }
    return dst / costsGraph.numVert();
}

template<typename Z>
static double calculateOptCoolingRate(size_t  n){
    return 1 - pow(100, -log10(n) + 1);
}

template <typename T>
class Annealing {

    public:

        explicit Annealing(const GrafoP<T>& costsGraph, bool randomInicialization = true): Annealing(costsGraph,
                                                                   calculateOptCoolingRate<T>(costsGraph.numVert()),
                                                                   avgDistance(costsGraph),
                                                                   int(100 * (1 / (1 - calculateOptCoolingRate<T>(costsGraph.numVert())))),
                                                                   randomInicialization) {};

        explicit Annealing(const GrafoP<T>& costsGraph, double coolingRate, double temperature,
                           int maxRepeats, bool randomInicialization): costsGraph(costsGraph),
                            tempSolution(costsGraph.numVert()),
                            coolingRate(coolingRate),
                            temperature(temperature),
                            maxRepeats(maxRepeats){

            //std::cout << "Annealing creado con temperatura inicial: " << temperature << ", tasa de enfriamiento: " << coolingRate << ", y max. repeticiones: " << maxRepeats << std::endl;

            init(randomInicialization);
        };

    std::pair<vector<typename GrafoP<T>::vertice>, double> findSolution();
    size_t iters() const;
    size_t maxIters() const;

    private:
        void init(bool randomInit);

        void randomInicialization();
        void greedyInicialization();

        void evolve();
        double evaluate();

        double getProbability(double cost);

        std::pair<typename GrafoP<T>::vertice, typename GrafoP<T>::vertice> getCities2Swap();

        double cost(typename GrafoP<T>::vertice cityA, typename GrafoP<T>::vertice cityB);

        const GrafoP<T>& costsGraph;
        std::vector<typename GrafoP<T>::vertice> tempSolution;
        double coolingRate;
        double temperature;
        size_t maxRepeats;
        size_t totalIters = 0;

        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution;
        std::uniform_real_distribution<double> realDistribution{0.0, 1.0};

};

#endif //PRACTICAS_ANNEALING_HPP
