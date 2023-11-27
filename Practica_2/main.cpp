
#include "generator.cpp"
#include "translator.cpp"
#include "iostream"
#include "solver.cpp"
#include "chrono"

/**
	El archivo contiene dos funciones principales:
		- realWork()
		- specificExample()
	
	La primera función realiza todos los pasos que se describen en la práctica: generación de instancias del problema y 
	guardado en sus respectivos ficheros, lectura de los ficheros y conversión a estándar DIMACS (también almacenándolos en sus respectivos ficheros), 
	lectura de los ficheros DIMACS, resolución del problema, muestra por pantalla de la solución.
	
	La segunda función sirve de prueba para ver que un caso concreto devuelva una solución correcta (hay como ejemplo en la carpeta "tests" tres ejemplares
	pequeños hechos a mano). Para cambiar el test hay que cambiar el nombre del fichero directamente dentro de la función.
*/

void printSolution(const std::vector<color>& solution){

    for (int i = 0; i < solution.size(); ++i) {
        std::cout << "La asignatura " << (i+1) << " hará el exámen el día " << solution[i] << std::endl;
    }
}

void realWork(){

    int initialN = 5;
    int lastN = 30;
    int step = 5;
    int nStudents = 500;

    // Generamos las instancias del problema y las guardamos
    experiment* experiment = batchGenerator(initialN, lastN, step, nStudents);
    saveExperiment(*experiment, "./instances/");
    delete experiment;


    /* Leemos los archivos del experimento */
    convertInstances2Dimacs("./instances/", "./dimacs/");


    // Lee fichero DIMACS y crea un grafo

    Grafo* grafo;
    std::string inputDir("./dimacs/");
    std::ifstream inputFile;
    DIR *dr;
    struct dirent *en;
    dr = opendir(inputDir.c_str());
    if (dr) {
        while ((en = readdir(dr)) != NULL) {

            // Convertimos nombre del fichero a string para mayor facilidad
            std::string fileName(en->d_name);

            if (fileName != "." && fileName != ".."){

                // Obtenemos la ruta absoluta del fichero
                std::string inputFileName(inputDir);
                inputFileName += fileName;

                // Leemos el fichero DIMACS y creamos un grafo
                inputFile.open(inputFileName);
                grafo = fromDimacsFile(inputFile);
                //std::cout << (*grafo) << std::endl;     // TODO DEBUG

                // --- Versión Optimizada ---
                // Procesamos el grafo (optimizado) y tomamos tiempo
                auto begin = std::chrono::high_resolution_clock::now();

                auto sol = solveOptimized(*grafo);

                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds >(end - begin);
                std::string measureUnit = " ms ";
                std::string measureNumber = std::to_string(duration.count() / 1000.0);
                if (duration.count() < 1000){
                    measureUnit = " micro segundos ";
                    measureNumber = std::to_string(duration.count());
                }

                // Mostramos solucion
                std::cout << "--- Solucion (Grafo de " << grafo->numVert() << " vertices) ---" << std::endl;
                std::cout << sol.first << std::endl;
                std::cout << "************" << std::endl;
                printVectorSolver(sol.second);
                std::cout << std::endl;
                std::cout << "************" << std::endl;
                std::cout << "Tiempo estimado " << measureNumber << measureUnit << std::endl;
                printSolution(sol.second);
                std::cout << " -------------- " << std::endl;




                /*
                // --- Versión sin optimizar ---
                // Procesamos el grafo (sin optimizar) y tomamos tiempo
                begin = std::chrono::high_resolution_clock::now();
                sol = solve(*grafo);
                end = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds >(end - begin);
                measureUnit = " ms ";
                measureNumber = std::to_string(duration.count() / 1000.0);
                if (duration.count() < 1000){
                    measureUnit = " micro segundos ";
                    measureNumber = std::to_string(duration.count());
                }

                std::cout << "--- Solucion (Grafo de " << grafo->numVert() << " vertices) ---" << std::endl;
                std::cout << sol.first << std::endl;
                std::cout << "************" << std::endl;
                printVectorSolver(sol.second);
                std::cout << std::endl;
                std::cout << "************" << std::endl;
                std::cout << "Tiempo estimado " << measureNumber << measureUnit << std::endl;
                printSolution(sol.second);
                std::cout << " -------------- " << std::endl;
                */

                inputFile.close();
                delete grafo;
            }
        }
    }
}

void specificExample(){

    Grafo* grafo;

    std::ifstream inputFile;
    inputFile.open("./tests/dimacs3_test.txt");
    grafo = fromDimacsFile(inputFile);
    inputFile.close();


    // Procesamos el grafo (sin optimizar) y tomamos tiempo
    auto begin = std::chrono::high_resolution_clock::now();
    auto sol = solveOptimized(*grafo);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds >(end - begin);
    std::string measureUnit = " ms ";
    std::string measureNumber = std::to_string(duration.count() / 1000.0);
    if (duration.count() < 1000){
        measureUnit = " micro segundos ";
        measureNumber = std::to_string(duration.count());
    }


    // Mostramos solucion
    std::cout << *grafo << std::endl;
    std::cout << "--- Solucion (Grafo de " << grafo->numVert() << " vertices) ---" << std::endl;
    std::cout << sol.first << std::endl;
    std::cout << "************" << std::endl;
    printVectorSolver(sol.second);
    std::cout << std::endl;
    std::cout << "************" << std::endl;
    std::cout << "Tiempo estimado " << measureNumber << measureUnit << std::endl;
    printSolution(sol.second);
    std::cout << " -------------- " << std::endl;


    /*
    // Procesamos el grafo (optimizado) y tomamos tiempo
    begin = std::chrono::high_resolution_clock::now();
    sol = solveOptimized(*grafo);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds >(end - begin);
    measureUnit = " ms ";
    measureNumber = std::to_string(duration.count() / 1000.0);
    if (duration.count() < 1000){
        measureUnit = " micro segundos ";
        measureNumber = std::to_string(duration.count());
    }

    std::cout << *grafo << std::endl;
    std::cout << "--- Solucion (Grafo de " << grafo->numVert() << " vertices) ---" << std::endl;
    std::cout << sol.first << std::endl;
    std::cout << "************" << std::endl;
    printVectorSolver(sol.second);
    std::cout << std::endl;
    std::cout << "************" << std::endl;
    std::cout << "Tiempo estimado " << measureNumber << measureUnit << std::endl;
    printSolution(sol.second);
    std::cout << " -------------- " << std::endl;
    */

    delete grafo;
}

int main(){

    // Función para probar casos en concreto
    //specificExample();

    // Esta función realiza todos los pasos necesarios: generacion de instancias del problema, guardado en archivo, lectura
    // de archivo y conversión a fichero DIMACS, lectura de fichero DIMACS, cálculo de la solución y muestra por pantalla de la solución.
    realWork();

    return 0;
}
