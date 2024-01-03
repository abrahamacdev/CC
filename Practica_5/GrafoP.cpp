//
// Created by abraham on 12/12/23.
//

#include "GrafoP.hpp"

#include "iomanip"

// Inserción de un Grafo en un flujo de salida.
template <typename T>
std::ostream& operator <<(std::ostream& os, const GrafoP<T>& G){
    typedef typename GrafoP<T>::vertice vertice;
    size_t n = G.numVert();
    os << n << " vertices" << std::endl;
    os << "   ";
    for (vertice j = 0; j< n; j++)
        os << std::setw(3) << j;
    os << std::endl;
    for (vertice i = 0; i < n; i++) {
        os << std::setw(3) << i;
        for (vertice j = 0; j < n; j++)
            os << std::setw(3) << G[i][j];
        os << std::endl;
    }
    return os;
}
