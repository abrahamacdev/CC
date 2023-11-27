//
// Created by abraham on 8/11/23.
//
#include "solver.h"
#include "numeric"
#include <bits/stdc++.h>
#include "algorithm"


template <typename T>
void printVectorSolver(std::vector<T>& v){
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
}

template <typename T>
void printSetSolver(std::set<T>& v){
    auto it = v.cbegin();
    while (it != v.cend()){
        std::cout << *it << " ";
        it++;
    }
    std::cout << std::endl;
}

bool existColorUnderBound(const std::set<color>& colores, size_t B){

    bool existUnderBound = false;

    // Todos los colores tienen que ser menor o igual que B
    auto i = colores.cbegin();
    while (!existUnderBound && i != colores.cend()){
        if (*i < B) existUnderBound = true;
        i++;
    }

    return existUnderBound;
}

std::vector<Grafo::vertice>* sortVerticesByDegree(Grafo* g){

    auto* verticesDegrees = g->grados();
    auto* sortedVertices = new std::vector<Grafo::vertice>(g->numVert());


    // Inicializamos sortedVertices de 0 hasta n-1
    std::iota(sortedVertices->begin(),sortedVertices->end(),0);


    // Ordenamos sortedVertices según los grados de cada vertices de verticesDegrees
    std::sort(sortedVertices->begin(), sortedVertices->end(),
              [&](int i,int j){ return (*verticesDegrees)[i] > (*verticesDegrees)[j]; } );

    // Eliminamos el vector de grados inicial
    delete verticesDegrees;

    return sortedVertices;
}

std::pair<size_t, std::vector<color>> solveRec(const Grafo& g,
                                                 std::set<Grafo::vertice> vertices,
                                                 size_t k,
                                                 size_t B,
                                                 std::vector<color>& bestSolution,
                                                 std::vector<std::set<color>> colours){

    // No hay más vertices que procesar
    if (vertices.empty()){
        return {k, bestSolution};
    }

    // Comprobamos si en los nodos a procesar existe alguno que pueda usar un color menor que B
    auto it = vertices.begin();
    while (it != vertices.end()){
        if (!existColorUnderBound(colours[*it], B)) return {B, bestSolution};
        it++;
    }

    // Cogemos el primer vertice del vector y lo eliminamos del conjunto de vertices a procesar
    Grafo::vertice v = *vertices.begin();
    vertices.erase(v);

    std::vector<color>& temporalBestSolution = bestSolution;
    std::vector<color> bestColoursFoundCopy(bestSolution);

    // Recorremos los colores asignables al vertice
    auto coloursIt = colours[v].begin();
    while (coloursIt != colours[v].end()){

        color c = *coloursIt;
        coloursIt++;

        // El color no supera el límite
        if (c < B){

            // Guardamos el color que le estableceríamos
            bestColoursFoundCopy[v] = c;

            // Creamos copia del conjunto de colours para evitar tener que quitar el color escogido y despues añadirlo
            // Tambien evitamos quitar al vertice del conjunto original
            std::vector<std::set<color>> colorsCopy(colours);

            // Eliminamos el color c del conjunto de colours disponibles de los adyacentes de v
            auto* adyacentes = g.adyacentes(v);
            for(auto vAdyacente : *adyacentes){
                if (vertices.find(vAdyacente) != vertices.end()) colorsCopy[vAdyacente].erase(c);
            }

            auto res = solveRec(g, vertices, std::max(k, c), B, bestColoursFoundCopy, colorsCopy);

            // Se ha mejorado la solución parcial => actualizamos los mejores colores
            if (res.first < B){
                B = res.first;
                temporalBestSolution = res.second;
            }

            // Que no se olvide eliminar el vector de los adyacentes
            delete adyacentes;
        }
    }

    return {B, temporalBestSolution};
}

std::pair<size_t, std::vector<color>> solveRecOptimized(const Grafo& g,
                                               std::set<Grafo::vertice>& vertices,
                                               std::vector<Grafo::vertice>& sortedVertices,
                                               size_t k,
                                               size_t B,
                                               std::vector<color>& bestSolution,
                                               std::vector<std::set<color>> colours){

    // No hay más vertices que procesar
    if (sortedVertices.empty()){
        return {k, bestSolution};
    }

    // Comprobamos si en los nodos a procesar existe alguno que pueda usar un color menor que B
    auto it = sortedVertices.begin();
    while (it != sortedVertices.end()){
        if (!existColorUnderBound(colours[*it], B)) return {B, bestSolution};
        it++;
    }

    // Cogemos el primer vertice del set y lo eliminamos del conjunto de vertices a procesar
    Grafo::vertice v = *sortedVertices.begin();
    sortedVertices.erase(sortedVertices.begin());       // ELiminamos el vertice v de sortedVertices
    vertices.erase(v);                                      // Eliminamos el vertice v de vertices

    std::vector<color>& temporalBestSolution = bestSolution;
    std::vector<color> bestColoursFoundCopy(bestSolution);

    // Recorremos los colores asignables al vertice
    auto coloursIt = colours[v].begin();
    while (coloursIt != colours[v].end()){

        color c = *coloursIt;
        coloursIt++;

        // El color no supera el límite
        if (c < B){

            // Guardamos el color que le estableceríamos
            bestColoursFoundCopy[v] = c;

            // Creamos copia del conjunto de colours para evitar tener que quitar el color escogido y despues añadirlo
            // Tambien evitamos quitar al vertice del conjunto original
            std::vector<std::set<color>> colorsCopy(colours);

            // Eliminamos el color c del conjunto de colours disponibles de los adyacentes de v
            auto* adyacentes = g.adyacentes(v);
            for(auto vAdyacente : *adyacentes){
                if (vertices.find(vAdyacente) != vertices.end()) colorsCopy[vAdyacente].erase(c);
            }

            auto res = solveRecOptimized(g, vertices, sortedVertices, std::max(k, c), B, bestColoursFoundCopy, colorsCopy);

            // Se ha mejorado la solución parcial => actualizamos los mejores colores
            if (res.first < B){
                B = res.first;
                temporalBestSolution = res.second;
            }

            // Que no se olvide eliminar el vector de los adyacentes
            delete adyacentes;
        }
    }

    // Volvemos a insertar el vertice en sortedVertices y vertices para evitar copias de conjuntos
    sortedVertices.insert(sortedVertices.begin(), v);
    vertices.insert(v);

    return {B, temporalBestSolution};
}

std::pair<size_t, std::vector<color>> solve(Grafo& g){

    // Vector con vertices de 0 a g.numVert()
    std::set<Grafo::vertice> vertices;
    for (size_t i = 0; i < g.numVert(); ++i) {
        vertices.insert(i);
    }

    // "Conjunto de conjuntos" con los colores posibles
    std::vector<std::set<color>> vectoresColores;

    // Vector con los posibles colores
    std::set<color> colores;
    for(size_t i = 1; i <= g.numVert(); i++){
        colores.insert(i);
    }

    // Guardamos los conjuntos de colores para cada vertice
    for (int i = 0; i < g.numVert(); ++i) {
        vectoresColores.push_back(std::set<color>(colores));
    }

    // Nuestro grafo tiene los vertices desde 0 hasta n-1.
    // Tambien los colores van desde 0 hasta n-1.
    // => La B que recibamos está desplazada 1 ud a la izquierda => B+1
    std::vector<color> bestSolution(g.numVert());
    auto res = solveRec(g, vertices, 0, g.numVert()+1, bestSolution, vectoresColores);
    return {res.first, res.second};
}

std::pair<size_t, std::vector<color>> solveOptimized(Grafo& g){

    // Vector con vertices de 0 a g.numVert()
    std::set<Grafo::vertice> vertices;
    for (size_t i = 0; i < g.numVert(); ++i) {
        vertices.insert(i);
    }

    // "Conjunto de conjuntos" con los colores posibles
    std::vector<std::set<color>> vectoresColores;

    // Vector con los posibles colores
    std::set<color> colores;
    for(size_t i = 1; i <= g.numVert(); i++){
        colores.insert(i);
    }

    // Guardamos los conjuntos de colores para cada vertice
    for (int i = 0; i < g.numVert(); ++i) {
        vectoresColores.push_back(std::set<color>(colores));
    }

    // Nuestro grafo tiene los vertices desde 0 hasta n-1.
    // Tambien los colores van desde 0 hasta n-1.
    // => La B que recibamos está desplazada 1 ud a la izquierda => B+1
    std::vector<color> bestSolution(g.numVert());

    // Obtenemos el orden de los vertices segun su grado
    std::vector<Grafo::vertice>* sortedVertices = sortVerticesByDegree(&g);

    auto res = solveRecOptimized(g, vertices, *sortedVertices, 0, g.numVert()+1, bestSolution, vectoresColores);
    return {res.first, res.second};
}