/*--------------------------------------------------------*/
/* grafoMA.h                                              */
/*                                                        */
/* clase Grafo: Matriz de adyacencia de un grafo.         */
/*                                                        */
/* Estructuras de Datos no Lineales                       */
/* 2016 José Fidel Argudo                                */
/*--------------------------------------------------------*/
/*
Tipo público:

   Grafo::vertice // un valor entre 0 y Grafo::numVert()-1

Métodos públicos:

   explicit Grafo(size_t n);
      Grafo de n vértices sin aristas.

   template <typename T> Grafo(const GrafoP<T>& G);
      Constructor de conversión. Construye un Grafo a partir
      de uno ponderado representado mediante matriz de costes,
      es decir, cambiando la representaci�n a matriz de adyacencia.

   size_t numVert() const;
      Número de vértices

   const vector<bool>& operator [](vertice v) const;
   vector<bool>& operator [](vertice v);
      Vértices adyacentes a v. Fila v de la matriz de adyacencia.

   bool esDirigido() const;
      Indica si un grafo es dirigido (true) o no (false).

Sobrecarga de operador externo:

   std::ostream& operator <<(std::ostream& os, const Grafo& G);
      Inserción de un Grafo en un flujo de salida.

----------------------------------------------------------*/

#ifndef GRAFO_MADY_H
#define GRAFO_MADY_H
#include <vector>
#include <ostream>
#include <string>

using std::vector;

template <typename tCoste> class GrafoP;

class Grafo {
public:
   typedef size_t vertice;
   typedef std::pair<vertice, vertice> arista;

   explicit Grafo(size_t n, bool diagonal = false): Grafo(n, std::vector<bool>(n, true)) {}

   explicit Grafo(size_t n, const std::vector<bool>& justCopy, bool diagonal = false) {

       initializeAdy();

       if (diagonal) rellenaDiagonal();
   }

   size_t numVert() const {return ady.size();}
   const vector<bool>& operator [](vertice v) const {return ady[v];}
   vector<bool>& operator [](vertice v) {return ady[v];}
   bool esDirigido() const;
   std::vector<arista>* aristas() const;
   std::vector<Grafo::vertice>* adyacentes(vertice v) const;
   size_t grado(vertice v) const;
   std::vector<size_t>* grados() const;

private:
   vector< vector<bool> > ady;

   void initializeAdy();
   void rellenaDiagonal();
};

// Inserci�n de un Grafo en un flujo de salida.
std::ostream& operator <<(std::ostream& os, const Grafo& G);

#endif   // GRAFO_MADY_H
