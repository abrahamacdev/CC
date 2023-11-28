
#include "translator.cpp"

std::string* executeCommand(const std::string& command){

    // Open a pipe to the command
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    // Read the command output into a buffer
    std::array<char, 128> buffer{};
    auto result = new std::string();
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        *result += buffer.data();
    }

    // Close the pipe
    int status = pclose(pipe);
    if (status == -1) {
        throw std::runtime_error("pclose() failed!");
    }

    return result;
}

bool oracle(Grafo& g, int k){

    // Crea el fichero temporal
    std::ofstream file;
    std::string path = "temp.cnf";
    file.open(path, std::ofstream::trunc);

    // Guarda el fichero
    toSATDimacsFile(file, g, k);

    // Cerramos el fichero
    file.flush();
    file.close();

    // Executamos PicoSAT e interpretamos la salida
    std::string* resultString = executeCommand("picosat temp.cnf");
    bool isSolvable = (*resultString)[2] == 'S';

    // Eliminamos de memoria la cadena
    delete resultString;

    // Eliminamos el fichero temporal
    std::remove(path.c_str());

    return isSolvable;
}

std::vector<Grafo::vertice>* searchVersion(Grafo& g, int k, bool printSolution = true){

    bool solvable = oracle(g, k);
    auto finalSolution = new std::vector<Grafo::vertice>();

    // Buscamos la solución
    if (solvable){

        std::vector<bool> toUse(g.numVert(), true);

        // Buscamos los nodos que conforman la solución
        int i = 0;
        int nSelectedNodes = 0;
        while (i<g.numVert() && nSelectedNodes != k){
            toUse[i] = false;

            // Creamos un grafo que no use al nodo i
            Grafo tempGraph = Grafo(g, toUse);

            // Si no puede resolverse quitando el nodo i es porque es necesario para la solucion
            if (!oracle(tempGraph, k)){
                finalSolution->push_back(i);
                toUse[i] = true;
            }

            i++;
        }
    }


    if (printSolution){
        if (finalSolution->empty()) std::cout << "No existe un clique de tamaño " << k << " para el grafo elegido" << std::endl;
        else {
            std::cout << "Una posible solucion es (nodos):\n";
            printVector(*finalSolution);
            std::cout << std::endl;
        }
    }

    return finalSolution;
}

std::vector<Grafo::vertice>* optimizationVersion(Grafo& g){

    auto* finalSolution = new std::vector<Grafo::vertice>();

    // Buscamos el mayor valor de k
    bool solvable = false;
    int k = g.numVert()+1;
    while (!solvable && k > 0){
        k--;
        solvable = oracle(g, k);
    }

    // Buscamos la solución
    if (solvable){
        finalSolution = searchVersion(g, k, false);
    }

    if (finalSolution->empty()) std::cout << "No existe un clique de tamaño " << k << " para el grafo elegido" << std::endl;
    else {
        std::cout << "El mayor clique que puede formarse tiene tamaño " << k << " y una de las posibles soluciones es (nodos):\n";
        printVector(*finalSolution);
        std::cout << std::endl;
    }

    return finalSolution;
}

Grafo* readGraph(const std::string& path){
    std::ifstream fileIn(path);
    Grafo* g = fromGraphDimacsFile(fileIn);
    fileIn.close();

    return g;
}

void saveGraph(Grafo& g, const std::string& path){

    // Crea el fichero temporal
    std::ofstream file;
    file.open(path, std::ofstream::trunc);

    // Guarda el fichero
    toGraphDimacsFile(file, g);

    // Cerramos el fichero
    file.flush();
    file.close();
}

int main(){

    /*
    Grafo g(4);
    g[0][1] = g[1][0] = 1;
    g[0][2] = g[2][0] = 1;
    g[1][3] = g[3][1] = 1;
    g[3][2] = g[2][3] = 1;
    */

    auto RUTA_GRAFO_DIMACS = "temp.dimacs";


    Grafo* g = readGraph(RUTA_GRAFO_DIMACS);

    // Muestra el grafo por pantalla
    //std::cout << *g << std::endl;

    // Versión Búsqueda
    int k = 2;
    searchVersion(*g, k);

    // Versión Optimización
    //optimizationVersion(*g);

    return 0;
}
