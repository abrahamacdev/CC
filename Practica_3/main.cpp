
#include "translator.cpp"
#include "grafoMA.cpp"
#include "solvers.cpp"

Grafo* readDimacsGraphFile(const std::string& path){
    std::ifstream fileIn(path);
    Grafo* g = fromGraphDimacsFile(fileIn);
    fileIn.close();

    return g;
}

void saveGraph2DimacsFile(Grafo& g, const std::string& path){

    // Crea el fichero temporal
    std::ofstream file;
    file.open(path, std::ofstream::trunc);

    // Guarda el fichero
    toGraphDimacsFile(file, g);

    // Cerramos el fichero
    file.flush();
    file.close();
}

Grafo* grafoPrueba(){

    auto *g = new Grafo(7);
    (*g)[0][1] = (*g)[1][0] = true;
    (*g)[0][2] = (*g)[2][0] = true;
    (*g)[0][4] = (*g)[4][0] = true;
    (*g)[0][5] = (*g)[5][0] = true;

    (*g)[1][3] = (*g)[3][1] = true;
    (*g)[1][4] = (*g)[4][1] = true;
    (*g)[1][6] = (*g)[6][1] = true;

    (*g)[2][3] = (*g)[3][2] = true;
    (*g)[2][5] = (*g)[5][2] = true;
    (*g)[2][6] = (*g)[6][2] = true;

    (*g)[3][4] = (*g)[4][3] = true;
    (*g)[3][6] = (*g)[6][3] = true;

    (*g)[4][5] = (*g)[5][4] = true;
    (*g)[4][6] = (*g)[6][4] = true;

    (*g)[5][6] = (*g)[6][5] = true;

    return g;
}

int main(){

    // TODO Modificar con los valores a usar
    auto RUTA_GRAFO_DIMACS = "./tests/complejo.dimacs";
    int K = 3;

    // Lee el archivo a procesar
    Grafo* g = readDimacsGraphFile(RUTA_GRAFO_DIMACS);

    //Grafo* g = grafoPrueba();
    //saveGraph2DimacsFile(*g, RUTA_GRAFO_DIMACS);

    // Muestra el grafo por pantalla
    //std::cout << *g << std::endl;



    // Versión Decisión
    //decisionVersion(*g, K);

    // Versión Búsqueda
    //searchVersion(*g, K);

    // Versión Optimización
    optimizationVersion(*g);

    return 0;
}
