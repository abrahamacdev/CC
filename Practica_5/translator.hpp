//
// Created by abraham on 12/12/23.
//

#ifndef PRACTICAS_TRANSLATOR_HPP
#define PRACTICAS_TRANSLATOR_HPP

#include "GrafoP.hpp"

GrafoP<double>* readFromTSPFile(const std::string& path);

std::vector<GrafoP<double>::vertice>* readFromTSPTourFile(const std::string& path);

#endif //PRACTICAS_TRANSLATOR_HPP