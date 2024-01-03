//
// Created by abraham on 12/12/23.
//

#ifndef PRACTICAS_GRAFOP_HPP
#define PRACTICAS_GRAFOP_HPP

#include "vector"
#include "iostream"

using std::vector;

template <typename T>
class GrafoP {

    public:

        typedef size_t vertice;

        explicit GrafoP<T>(size_t n): g(n, vector<T>(n, T())){};
        explicit GrafoP<T>(const GrafoP<T>& p): g(p.g) {};

        size_t numVert() const {return g.size();}
        const vector<T>& operator [](vertice v) const {return g[v];}
        vector<T>& operator [](vertice v) {return g[v];}

    private:
        vector<vector<T>> g;

};

template <typename T>
std::ostream& operator <<(std::ostream& os, const GrafoP<T>& G);

#endif //PRACTICAS_GRAFOP_HPP
