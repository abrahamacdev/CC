
#include <math.h>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "generator.h"


std::ostream& operator << (std::ostream& os, const instance& instance){

    
    int asignatura = 1;

    for (size_t j = 0; j < instance.size(); j++){
        
        subject* subject = instance[j];
        
        // Escribimos el id de la asignatura
        os << asignatura++ << " ";

        //std::cout << "La asignatura " << (asignatura-1) << " tiene " << subject->size() << " alumnos" << std::endl;

        // Recorremos los alumnos de la asignatura
        for (size_t i = 0; i < subject->size(); i++){
            
            // Escribimos el id del alumno
            os << (*subject)[i];
            
            // Añadimos separador
            if (i != subject->size() - 1){
                os << " ";
            }
            
        }
        
        // Añadimos el salto de línea entre asignaturas
        if(j != instance.size() - 1) os << std::endl;
    }
    
    return os;
}


/**
 * Genera un ejemplo concreto del problema de la planificación de exámenes con k asignaturas
 * y un total de nStudents alumnos matriculables.
*/
instance& singleGenerator(int k, int nStudents){

    // Creamos un vector con los números de los estudiantes;
    std::vector<int> studentsVector(nStudents);
    std::iota (std::begin(studentsVector), std::end(studentsVector), 0);

    // Mezclamos los alumnos
    std::random_shuffle(studentsVector.begin(), studentsVector.end());
    
    // Generamos el vector con las distintas asignaturas
    instance& subjectsInstance = (*new instance(k));

    // Asignamos los alumnos a cada asignatura
    for (size_t i = 1; i <= k; i++){
        
        // Calculamos el número de alumnos de la asignatura
        double P_k = 1.0/std::ceil((double)i/10.0);
        int actualNStudents = std::ceil(nStudents/2 * P_k);

        // Cogemos los alumnos de la asignatura y los guardamos        
        subjectsInstance[i-1] = new subject(studentsVector.begin(), studentsVector.begin() + actualNStudents);


        // Volvemos a mezclar los alumnos
        std::random_shuffle(studentsVector.begin(), studentsVector.end());
    }
    
    //std::cout << "Se han generado " << subjectsInstance.size() << " asignaciones para un experimento" << std::endl;

    return subjectsInstance;
}

/**
 * Genera un conjunto de ejemplares del problema de la planificación de exámenes.
 * @param initialN: Valor inicial para la creación de ejemplares (aka nº de asignaturas del problema).
 * @param lastN: Valor final para la creación de ejemplares (aka nº de asignaturas del problema).
 * @param step: Incremento de asignaturas entre ejemplares. Ejemplo: initialN=4, lastN=10, steps=2 => ejemplares de: 4, 6, 8 y 10 asignaturas cada uno.
 * @param nStudents: Nº de estudiantes totales a matricular en las asignaturas.
*/
experiment* batchGenerator(int initialN, int lastN, int step, int nStudents){

    // Creamos un vector con los números de los estudiantes;
    int n = (lastN - initialN) / step;

    std::cout << "Generando ejemplares..." << std::endl;

    // Creamos varias asignaciones
    experiment* examples = new experiment(n+1);

    // Creamos cada una de las asignaciones
    int vecPos = 0;
    for (size_t i = initialN; i <= lastN; i += step){

        (*examples)[vecPos] = &singleGenerator(i, nStudents);
        vecPos++;
    }

    std::cout << "Ejemplares generados: " << examples->size() << std::endl;

    return examples;
}


void saveExperiment(experiment& experiment, const std::string& dir){

    int indxInstance = 1;

    for (size_t i = 0; i < experiment.size(); i++){

        // Guardamos cada instancia en un fichero separado
        instance* instance = experiment[i];

        std::ofstream file;
        std::string path = dir + std::string("instance") + std::to_string(indxInstance++) + ".txt";
        file.open(path, std::ofstream::trunc);

        std::cout << "Guardando instancia " << (indxInstance-1) << " en '" << path << "'..." << std::endl;

        // Guardamos la instancia en un fichero
        file << (*instance);

        // Cerramos el fichero 
        file.flush();
        file.close();
    }
}