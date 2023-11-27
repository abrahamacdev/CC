#include "translator.h"
#include "grafoMA.cpp"
#include <bits/stdc++.h>
#include <dirent.h>

template <typename T>
void printVector(std::vector<T>& v){
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}

void addEdges(Grafo& g, std::vector<int>& asignments){

    for (size_t i = 0; i < asignments.size()-1; i++){
        for (size_t j = i+1; j < asignments.size(); j++){
            g[asignments[i]][asignments[j]] = g[asignments[j]][asignments[i]] = true;
        }   
    }
}


void convertInstances2Dimacs(const std::string& inputDir, const std::string& outputDir){


    std::ofstream outputFile;
    std::ifstream inputFile;

    DIR *dr;
    struct dirent *en;

    dr = opendir(inputDir.c_str()); //open all directory
    if (dr) {
        while ((en = readdir(dr)) != NULL) {

            // COnvertimos nombre del fichero a string para mayor facilidad
            std::string fileName(en->d_name);

            // El nombre del fichero no es '.' ni '..'
            if (fileName != "." && fileName != ".."){

                // Ruta del fichero DIMACS de salida
                std::string outputFileName = outputDir + std::string("dimacs_").append(fileName);

                // Ruta del fichero con la instancia del problema de entrada
                std::string inputFileName = inputDir + fileName;

                // Leemos la instancia del problema
                inputFile.open(inputFileName);
                Grafo* p = fromInstanceFile(inputFile);
                inputFile.close();

                // Lo guardamos en su fichero dimacs
                outputFile.open(outputFileName, std::ios::trunc);
                toDimacsFile(outputFile, *p);
                outputFile.close();
            }
        }
        closedir(dr); //close all directory
    }
}

Grafo* fromInstanceFile(std::ifstream& in){

    std::map<int, std::vector<int>>& alumnAssignment = (*new std::map<int, std::vector<int>>());
    int countedAssignments = 0;

    std::string temp;
    
    // Leemos cada línea del archivo
    while (getline(in, temp)){

        std::stringstream ss(temp);
        std::string number;
        int asignatura = -1;

        // Separamos por espacios
        while(getline(ss, number, ' ')){
            
            int id = std::stoi(number);

            // Id de la asignatura   
            if(asignatura == -1) asignatura = id-1;
            // Id de alumno
            else{
                
                // No se ha introducido hasta ahora al alumno => Inicializamos el vector
                if(alumnAssignment.find(id) == alumnAssignment.end()){
                    alumnAssignment.insert({id, {asignatura}});
                }

                // Hay otra asignatura que tiene matriculado al alumno => Añadimos el id de la asignatura
                else {
                    alumnAssignment[id].push_back(asignatura);
                }
            }
        }

        countedAssignments++;
    }

    // Inicializamos el grafo
    Grafo* g = new Grafo(countedAssignments, false);

    // TODO DEBUG
    for(auto it = alumnAssignment.begin(); it != alumnAssignment.end(); it++){
        if (it->second.size() > 1){

            // TODO DEBUG
            /*
            std::cout << "El alumno " << it->first << " está matriculado en (";
            std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<int>(std::cout, " "));
            std::cout << ")" << std::endl;
            */

            // Unimos las asignaturas adyacentes
            addEdges(*g, it->second);
        }
    }

    // TODO DEBUG
    //std::cout << (*g) << std::endl;

    return g;
}

void toDimacsFile(std::ofstream& os, Grafo& g){

    int nVertex = g.numVert();
    int nEdges = 0;

    std::string headerString = std::string("p edge ") + std::to_string(nVertex) + std::string(" ");
    std::string edgesString = "";

    // Recorremos solo la diagonal superior
    for (int i = 0; i < g.numVert(); ++i) {

        // Guardamos el valor de la diagonal principal
        edgesString += std::string("e ") + std::to_string(i) + std::string(" ") + std::to_string(i) + std::string("\n");
        nEdges++;

        for (int j = i+1; j < g.numVert(); ++j) {
            if (g[i][j]){
                edgesString += std::string("e ") + std::to_string(i) + std::string(" ") + std::to_string(j) + std::string("\n");
                edgesString += std::string("e ") + std::to_string(j) + std::string(" ") + std::to_string(i) + std::string("\n");
                nEdges += 2;
            }
        }
    }
    edgesString += std::string("c Y se acabo");

    // Añadimos a la cabecera el número de aristas a parsear
    headerString += std::to_string(nEdges) + std::string("\n");

    // Guardamos la cabecera y las aristas
    os << headerString;
    os << edgesString;
}

Grafo* fromDimacsFile(std::ifstream& in){

    Grafo* g = nullptr;

    std::string line;

    while (getline(in, line)){

        // TODO DEBUG
        //std::cout << line << std::endl;

        // Inicializamos el grafo con la cabecera
        if (g == nullptr){

            // Buscamos el 3er campo de la cabecera que será el que contenga el número de vertices
            int indx = 1;
            std::stringstream ss(line);
            std::string trash;
            while (getline(ss, trash, ' ')){
                if (indx == 3){
                    int nEdges = std::stoi(trash);
                    g = new Grafo(nEdges);
                }
                indx++;
            }
        }

        // Extraemos la informacion del arista
        else {

            // 2do y 3er campo son el origen y destino del arista
            std::stringstream ss(line);
            std::string trash;
            int from, to;

            // Leemos los campos
            ss >> trash >> from >> to;

            // TODO Comentar. Lee los archivos DIMACS de tal manera que cada arista será guardada en ambos sentidos (Grafo no dirigido)
            /*
            from--;
            to--;
            (*g)[from][to] = (*g)[to][from] = true;
             */

            // Guarda cada arista individualmente (Grafo dirigido)
            (*g)[from][to] = true;        // TODO Descomentar
        }
    }

    return g;
}