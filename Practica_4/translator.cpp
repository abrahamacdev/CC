//
// Created by abraham on 12/12/23.
//

#include "translator.hpp"
#include "fstream"
#include "iostream"
#include "sstream"
#include "math.h"

template <typename T>
void printVector(std::vector<T>& v){
    for (int i = 0; i < v.size()-1; ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << v[v.size()-1] << std::endl;
}

double euclideanDistance(std::pair<int, int> xy1, std::pair<int, int> xy2){
    return std::sqrt(std::pow(xy1.first - xy2.first, 2) + std::pow(xy1.second - xy2.second, 2));
}

vector<std::string> parseLine(const std::string& line) {
    vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

GrafoP<double>* parseTSPFile(std::ifstream& file){

    GrafoP<double>* g = nullptr;

    vector<std::pair<int, int>> nodeCoordinates;

    // Lee las posiciones de las ciudades en el mapa
    std::string line;
    bool readingNodes = false;
    bool iterate = true;

    while (iterate && getline(file, line)) {

        vector<std::string> tokens = parseLine(line);

        if (!tokens.empty()) {

            // Buscamos la sección 'NODE_COORD_SECTION'
            if (tokens[0] == "NODE_COORD_SECTION") {
                readingNodes = true;
            }
            else {

                if (readingNodes) {

                    // Ya no hay nada más que procesar
                    if (tokens[0] == "EOF") {
                        iterate = false;
                    }

                    // Parseamos los números
                    else {

                        int nodeId = stoi(tokens[0]);
                        int xCoord = stoi(tokens[1]);
                        int yCoord = stoi(tokens[2]);

                        nodeCoordinates.emplace_back(xCoord, yCoord);
                    }
                }
            }
        }
    }

    // Inicializamos el grafo
    g = new GrafoP<double>(nodeCoordinates.size());

    // Crea un grafo con la distancia entre cada ciudad
    for (size_t i = 0; i < nodeCoordinates.size() - 1; i++) {
        (*g)[i][i] = 0;
        for (size_t j = i+1; j < nodeCoordinates.size(); j++) {
            (*g)[i][j] = (*g)[j][i] = euclideanDistance(nodeCoordinates[i], nodeCoordinates[j]);
        }
    }

    return g;
};

GrafoP<double>* readFromTSPFile(const std::string& path){

    GrafoP<double>* g = nullptr;

    std::ifstream file;
    file.exceptions ( std::ifstream::badbit ); // No need to check failbit
    try{

        // Abrimos el archivo
        file.open(path);

        g = parseTSPFile(file);

        // Cerramos el flujo
        file.close();

    }catch (const std::ifstream::failure& e) {
        std::cout << "Ocurrió un error al abrir el archivo '" << path << "'" << std::endl;
    }
    return g;
}

vector<GrafoP<double>::vertice>* parseTSPTourFile(std::ifstream& file){

    vector<GrafoP<double>::vertice>* g = new vector<GrafoP<double>::vertice>;

    // Lee las posiciones de las ciudades en el mapa
    std::string line;
    bool readingNodes = false;
    bool iterate = true;
    while (iterate && getline(file, line)) {
        vector<std::string> tokens = parseLine(line);

        if (!tokens.empty()) {

            // Buscamos la sección 'TOUR_SECTION'
            if (tokens[0] == "TOUR_SECTION") {
                readingNodes = true;
            }
            else {

                if (readingNodes) {

                    // Ya no hay nada más que procesar
                    if (tokens[0] == "-1") {
                        iterate = false;
                    }

                    // Parseamos los números
                    else {

                        int idNodo = stoi(tokens[0]);
                        g->push_back(idNodo);
                    }
                }
            }
        }
    }

    return g;
};

vector<GrafoP<double>::vertice>* readFromTSPTourFile(const std::string& path) {

    vector<GrafoP<double>::vertice>* g = nullptr;

    std::ifstream file;
    file.exceptions ( std::ifstream::badbit ); // No need to check failbit
    try{

        // Abrimos el archivo
        file.open(path);

        g = parseTSPTourFile(file);

        // Cerramos el flujo
        file.close();

    }catch (const std::ifstream::failure& e) {
        std::cout << "Ocurrió un error al abrir el archivo '" << path << "'" << std::endl;
    }

    return g;
}