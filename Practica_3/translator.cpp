#include "translator.h"
#include <bits/stdc++.h>

int indx2cell(int fila, int col, int nVertices){
    return fila * nVertices + col;
}

void toGraphDimacsFile(std::ofstream& os, Grafo& g){

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

Grafo* fromGraphDimacsFile(std::ifstream& in){

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

void toSATDimacsFile(std::ofstream& os, Grafo& g, int k){

    int numVertices = g.numVert();
    int numClauses = 0;

    std::string final = "";

    // Cada vértice debe de estar conectado como mínimo a un nodo
    for (int i = 0; i < k; ++i) {
        std::string temp = "";
        for (int j = 0; j < numVertices; ++j) {

            if (j != 0){
                temp += " ";
            }

            temp += std::to_string(indx2cell(j, i, numVertices) + 1);
        }
        final += temp + " 0\n";
        numClauses++;
    }

    // Cada vértice sólo aparece una vez
    for (int i = 0; i < numVertices; ++i) {
        for (int r = 0; r < k-1; ++r) {
            for (int s = r + 1; s < k; ++s) {

                // Debug
                //std::cout << "X" << i+1 << "," << r+1 << " OR " << "X" << i+1 << "," << s+1 << std::endl;

                std::string string_ir = std::to_string(indx2cell(i, r, numVertices) + 1);
                std::string string_is = std::to_string(indx2cell(i, s, numVertices) + 1);

                final += "-" + string_ir + " -" + string_is + " 0\n";
                numClauses++;
                //os << "-" << string_ir << " -" << string_is << "0"<< std::endl;
            }
        }
    }

    // Evitamos seleccionar nodos que no estén conectado
    for (int i = 0; i < numVertices-1; ++i) {
        for (int j = i+1; j < numVertices; ++j) {

            // 1 <= i < j <= n && No hay arista que una (i,j)
            if (!g[i][j]){

                // 1 <= r,s <= k && r != s
                for (int r = 0; r < k; ++r) {
                    for (int s = 0; s < k; ++s) {
                        if (r!=s){

                            // Debug
                            //std::cout << "X" << i+1 << "," << r+1 << " OR " << "X" << j+1 << "," << s+1 << std::endl;

                            std::string string_ir = std::to_string(indx2cell(i, r, numVertices) + 1);
                            std::string string_js = std::to_string(indx2cell(j, s, numVertices) + 1);

                            final += "-" + string_ir + " -" + string_js + " 0\n";
                            numClauses++;
                        }
                    }
                }
            }
        }
    }

    // Guardamos la información en el fichero
    final.pop_back();
    os << "p cnf " << numVertices * numVertices << " " << numClauses << std::endl;
    os << final;
}