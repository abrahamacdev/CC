#include "vector"

typedef std::vector<int> subject;               // Alumnos de la asignatura
typedef std::vector<subject*> instance;         // Asignaturas del ejemplar
typedef std::vector<instance*> experiment;      // Conjunto de ejemplos

typedef size_t color;