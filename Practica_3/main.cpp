
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

void searchVersion(Grafo& g, int k){
}

int main(){

    Grafo g(4);
    g[0][1] = g[1][0] = 1;
    g[0][2] = g[2][0] = 1;
    g[1][3] = g[3][1] = 1;
    g[3][2] = g[2][3] = 1;

    std::cout << g << std::endl;

    int k = 3;
    oracle(g, k);

    return 0;
}
