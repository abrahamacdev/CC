
#include "types.h"
#include <iostream>
#include <fstream>
#include "grafoMA.h"

/**
 * Convierte todos los archivos generados por generator.cpp a archivos DIMACS.
 * @param inputDir
 * @param outputDir
 */
void convertInstances2Dimacs(const std::string& inputDir, const std::string& outputDir);

/**
 * Lee un fichero escrito en notacion "exam instance" (una asignatura por fila. 
 * Lo primero es un identificador de la asignatura. El resto son identificadores de los alumnos. Todo está separado por espacios).
 * y crea un grafo de acuerdo al problem "graph colouring instance" (dos nodos son adyacentes cuando al menos uno de los alumnos
 * matriculados en ambas asignaturas sea el mismo)
*/
Grafo* fromInstanceFile(std::ifstream& in);

/**
 * Guarda un objeto de tipo Grafo a un fichero siguiendo el estándar DIMACS.
*/
void toDimacsFile(std::ofstream& os, Grafo& g);

/**
 * Lee un archivo con formato DIMACS y genera un objeto de tipo Grafo.
*/
Grafo* fromDimacsFile(std::ifstream& in);