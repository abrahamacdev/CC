
#include <iostream>
#include <fstream>
#include "grafoMA.h"

/**
 * Guarda un objeto de tipo Grafo a un fichero siguiendo el estándar DIMACS.
*/
void toGraphDimacsFile(std::ofstream& os, Grafo& g);

/**
 * Lee un archivo con formato DIMACS y genera un objeto de tipo Grafo.
*/
Grafo* fromGraphDimacsFile(std::ifstream& in);

/**
 * Guarda la información del grafo siguiendo el formato SAT CNF DIMACS
*/
void toSATDimacsFile(std::ofstream& os, Grafo& g);