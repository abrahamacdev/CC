//
// Created by abraham on 4/12/23.
//

#ifndef PRACTICAS_SOLVERS_H
#define PRACTICAS_SOLVERS_H

#include "grafoMA.h"
#include "translator.h"

bool decisionVersion(Grafo& g, int k);
std::vector<Grafo::vertice>* searchVersion(Grafo& g, int k, bool printSolution = true);
std::vector<Grafo::vertice>* optimizationVersion(Grafo& g);

#endif //PRACTICAS_SOLVERS_H
