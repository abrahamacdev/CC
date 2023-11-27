/*--------------------------------------------------------*/
/* grafoMA.cpp                                            */
/*                                                        */
/* Fichero de implementación de la clase Grafo (matriz de */
/* adyacencia).                                           */
/*                                                        */
/* Estructuras de Datos no Lineales                       */
/* ©2016 José Fidel Argudo                                */
/*--------------------------------------------------------*/

#include "grafoMA.h"
#include <fstream>
#include <sstream>
#include <iomanip>

// Constructor desde fichero
/*
Grafo::Grafo(const std::string& nf)
{
   char c;
   std::string cad;
   vertice v, w;
   std::ifstream fg(nf);           // apertura del fichero
   unsigned n;                     // núm. vértices

   fg >> n;
   ady = vector< vector<bool> >(n, vector<bool>(n, false));
   while (fg >> v) {
      fg >> c; // extrae ':'
      std::getline(fg, cad);
      std::istringstream ss(cad);
      while (ss >> w)
         ady[v][w] = true;
   }
   fg.close();
}
*/

size_t countAllTrues(const std::vector<bool>& justCopy){
    size_t count = 0;
    for (int i = 0; i < justCopy.size(); ++i) {
        if (justCopy[i]) count++;
    }
    return count;
}

void Grafo::copyWithLimits(Grafo& g, const std::vector<bool>& justCopy){

    if (g.numVert() != justCopy.size()) throw std::runtime_error("Cannot create graph");

    // Determinamos el tamañoo del grafo final
    size_t n = std::min(g.numVert(), countAllTrues(justCopy));

    // Inicializamos
    ady = {n, vector<bool>(n, false)};

    int k = 0;
    for (int i = 0; i < g.numVert(); ++i) {

        // Se quiere usar el nodo i
        if (justCopy[i]){

            // Creamos las aristas que involucran al nodo i
            int r = 0;
            for (int j = 0; j < g.numVert(); ++j) {

                if (justCopy[j]){
                    ady[k][r] = ady[r][k] = g[i][j];
                    r++;
                }
            }

            k++;
        }
    }
}

// dirigido = true, no dirigido = false
bool Grafo::esDirigido() const
{
   bool s = true;   // matriz simétrica ==> no dirigido
   const size_t n = numVert();
   vertice i = 0;
   while (s && i < n) {
      vertice j = i+1;
      while (s && j < n) {
         s = (ady[i][j] == ady[j][i]);
         ++j;
      }
      ++i;
   }
   return !s;   // no simétrica ==> dirigido
}

void Grafo::rellenaDiagonal() {
    for(int i=0; i<ady.size(); i++) ady[i][i] = true;
}

vector<Grafo::arista>* Grafo::aristas() const {

    auto* aristas = new std::vector<Grafo::arista>();

    for (size_t i = 0; i < ady.size(); ++i) {
        for (size_t j = 0; j < ady.size(); ++j) {
            if (ady[i][j]) aristas->emplace_back(i, j);
        }
    }

    return aristas;
}

std::vector<Grafo::vertice> *Grafo::adyacentes(Grafo::vertice v) const {

    auto* vectorAdy = new std::vector<Grafo::vertice>();

    for (size_t i = 0; i < ady.size(); ++i) {
        if (ady[v][i] || ady[i][v]) vectorAdy->push_back(i);
    }

    return vectorAdy;
}

size_t Grafo::grado(Grafo::vertice v) const {

    size_t cuenta = 0;
    for (vertice i = 0; i < ady.size(); i++) {
        if (v != i && (ady[v][i] || ady[i][v])) cuenta++;
    }

    return cuenta;
}

std::vector<size_t> *Grafo::grados() const {

    auto* gradosVertices = new std::vector<size_t>(ady.size());

    for (vertice v=0; v < ady.size(); v++) {

        (*gradosVertices)[v] = grado(v);

    }

    return gradosVertices;
}

// Inserción de un Grafo en un flujo de salida.
std::ostream& operator <<(std::ostream& os, const Grafo& G)
{
   typedef Grafo::vertice vertice;
   const size_t n = G.numVert();
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
