//
// Created by abraham on 24/12/23.
//

#include "annealing.hpp"
#include <algorithm>
#include "set"
#include "math.h"
#include <random>

template <typename T>
void Annealing<T>::init(bool randomInit) {
    distribution = std::uniform_int_distribution<int>(0,costsGraph.numVert()-1);

    randomInit ? randomInicialization() : greedyInicialization();
}

template <typename T>
void Annealing<T>::randomInicialization() {

    std::vector<typename GrafoP<T>::vertice> myvector;

    for (size_t i=0; i<tempSolution.size(); ++i) myvector.push_back(i); // 1 2 3...

    // using built-in random generator:
    std::shuffle( myvector.begin(), myvector.end() , std::mt19937(std::random_device()()));

    tempSolution = myvector;
}

typedef std::pair<double, size_t> cost_index_pair;
bool comparator ( const cost_index_pair& l, const cost_index_pair& r) { return l.first < r.first; }

template <typename T>
vector<cost_index_pair>* indxVector(vector<T>& v){
    vector<cost_index_pair>* indexed = new vector<cost_index_pair >();
    for (size_t i = 0; i < v.size(); ++i) {
        indexed->emplace_back(v[i], i);
    }
    return indexed;
}

template <typename T>
void Annealing<T>::greedyInicialization() {

    vector<typename GrafoP<T>::vertice> path{};
    std::set<typename GrafoP<T>::vertice> selectedVertices;

    size_t origen = 0;
    size_t procesados = 1;
    path.emplace_back(origen);      // Añadimos el nodo de origen a la ruta
    selectedVertices.insert(origen);
    while (procesados != costsGraph.numVert()){

        // Obtenemos los costes de ir desde origen a cada una de las distintas ciudades
        vector<T> costs = costsGraph[origen];
        vector<cost_index_pair>& costsIndexed = (*indxVector(costs));
        std::sort (costsIndexed.begin(), costsIndexed.end(), comparator);

        // Buscamos el nodo más cercano al origen
        bool loop = true;
        size_t i = 0;
        while (loop){
            size_t destination = costsIndexed[i].second;

            // Origen != Destino y Destino no puede ser un nodo ya procesado
            if (destination != origen && selectedVertices.find(destination) == selectedVertices.end()){
                origen = destination;
                loop = false;

                // Añadimos origen a la ruta y lo marcamos como visitado
                path.emplace_back(origen);
                selectedVertices.insert(origen);
            }

            i++;
        }

        procesados++;
    }

    tempSolution = path;
}

template <typename T>
void Annealing<T>::evolve(){

    if (totalIters == 0){

        size_t repetitions = 0;
        double lowestDist = -1.0;

        while (repetitions < maxRepeats){

            totalIters += 1;

            // Obtenemos las dos ciudades a intercambiar
            auto cities2Swap = getCities2Swap();

            // Calculamos el coste adicional
            double loss = cost(cities2Swap.first, cities2Swap.second);

            // Cambiamos la ruta si la nueva tiene un coste mejor o, en caso de ser peor, bajo una probabilidad.
            double uniform = realDistribution(generator);
            double prob = (loss <= 0 or temperature <= 0) ? 0 : getProbability(loss);
            if (loss <= 0 || uniform < prob){

                // Versión "intercambia cacho"
                // Invertimos el cacho que comprende [inicio+1, fin)
                //std::reverse(tempSolution.begin() + cities2Swap.first + 1, tempSolution.begin() + cities2Swap.second);


                // Versión "ointercambia ciudad individual"
                typename GrafoP<T>::vertice tempVertex = tempSolution[cities2Swap.first];
                tempSolution[cities2Swap.first] = tempSolution[cities2Swap.second];
                tempSolution[cities2Swap.second] = tempVertex;

            }

            temperature = temperature * coolingRate;

            // Comprobamos si la distancia de la ruta es menor que la menor hasta el momento
            double actualDist = evaluate();
            if (actualDist < lowestDist) {
                repetitions = 0;
                lowestDist = actualDist;
            }
            else {
                repetitions += 1;
            }
        }
    }
}

template<typename T>
size_t Annealing<T>::iters() const {
    return totalIters;
}

template <typename T>
double Annealing<T>::evaluate(){

    double cost = 0;
    for (size_t i = 0; i < tempSolution.size() - 1; ++i) {
        GrafoP<double>::vertice origen = tempSolution[i];
        GrafoP<double>::vertice destination = tempSolution[i+1];
        cost += costsGraph[origen][destination];
    }

    return cost;
}

template <typename T>
double Annealing<T>::getProbability(double cost){
    double prob = exp(-cost / temperature);
    return 1 < prob ? 1 : prob;
}

template <typename T>
std::pair<typename GrafoP<T>::vertice, typename GrafoP<T>::vertice> Annealing<T>::getCities2Swap(){

    typename GrafoP<T>::vertice origen = distribution(generator);
    typename GrafoP<T>::vertice destination = distribution(generator);


    return origen < destination ? std::make_pair(origen, destination) : std::make_pair(destination, origen);
}

template <typename T>
double Annealing<T>::cost(typename GrafoP<T>::vertice cityA, typename GrafoP<T>::vertice cityB){

    // Versión "intercambia cacho"
    /*
    size_t n = tempSolution.size();

    typename GrafoP<T>::vertice a1 = tempSolution[cityA];
    typename GrafoP<T>::vertice a2 = tempSolution[(cityA + 1) % n];

    typename GrafoP<T>::vertice b1 = tempSolution[cityB];
    typename GrafoP<T>::vertice b2 = tempSolution[(cityB + 1) % n];

    double actualCost = costsGraph[a1][a2] + costsGraph[b1][b2];
    double newCost = costsGraph[a1][b1] + costsGraph[a2][b2];
    */




    // Versión "intercambia ciudades individuales"
    // Calculamos el coste actual
    double actualCost = evaluate();

    //std::cout << "Cambio " << tempSolution[cityA] << " -- " << tempSolution[cityB] << std::endl;
    //printVector(tempSolution);

    typename GrafoP<T>::vertice tempVertex = tempSolution[cityA];
    tempSolution[cityA] = tempSolution[cityB];
    tempSolution[cityB] = tempVertex;

    // Calculamos el coste haciendo el cambio
    double newCost = evaluate();

    //printVector(tempSolution);
    //std::cout << "Coste actual: " << actualCost << std::endl;
    //std::cout << "Coste nuevo: " << newCost << std::endl;
    //std::cout << "_--------------------------" << std::endl;

    // Devolvemos toodo al estado original
    tempVertex = tempSolution[cityA];
    tempSolution[cityA] = tempSolution[cityB];
    tempSolution[cityB] = tempVertex;


    // Calculamos la diferencia entre el actual y el "nuevo"
    return newCost - actualCost;
}

template<typename T>
size_t Annealing<T>::maxIters() const {
    return maxRepeats;
}

template <typename T>
std::pair<vector<typename GrafoP<T>::vertice>, double> Annealing<T>::findSolution(){
    evolve();
    return std::make_pair(tempSolution, evaluate());
}