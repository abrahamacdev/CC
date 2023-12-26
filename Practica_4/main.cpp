//
// Created by abraham on 12/12/23.
//

#include <algorithm>
#include <dirent.h>
#include "GrafoP.cpp"
#include "translator.cpp"
#include "annealing.cpp"
#include "chrono"

double evaluateOptimal(std::vector<GrafoP<double>::vertice>& vec, GrafoP<double>& grafo){

    double cost = 0;
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        GrafoP<double>::vertice origen = vec[i]-1;
        GrafoP<double>::vertice destination = vec[i+1]-1;

        cost += grafo[origen][destination];
    }
    return cost;
}

double evaluate(std::vector<GrafoP<double>::vertice>& vec, GrafoP<double>& grafo){

    double cost = 0;
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        GrafoP<double>::vertice origen = vec[i];
        GrafoP<double>::vertice destination = vec[i+1];

        cost += grafo[origen][destination];
    }
    return cost;
}


typedef GrafoP<double> tspProblem;
typedef std::vector<GrafoP<double>::vertice> tspSolution;
typedef std::pair<tspProblem*, tspSolution*> tspInstance;
typedef std::pair<tspInstance, std::string> tspInfo;
typedef vector<tspInfo> listTSPInstances;

using namespace std::chrono;

listTSPInstances* loadAllTSPInstances(std::string problemsPath = "./problems/", std::string solutionsPath = "./solutions/"){

    struct dirent *entry;
    DIR *dir = opendir(problemsPath.c_str());
    if (dir == NULL) {
        return nullptr;
    }

    listTSPInstances* tspInstances = new listTSPInstances();
    while ((entry = readdir(dir)) != NULL) {

        std::string file = std::string(entry->d_name);

        if (file != "." && file != ".."){

            // Leemos las posiciones de las ciudades
            std::string problemName = entry->d_name;
            std::string rawProblemName = problemName.substr(0, problemName.find('.'));
            tspProblem* instance = readFromTSPFile(problemsPath + problemName);

            // Leemos la solución óptima
            std::string solutionFileName = rawProblemName + ".opt.tour";
            tspSolution* solution = readFromTSPTourFile(solutionsPath + solutionFileName);

            // Guardamos la instancia del problema junto con el nombre del problema
            tspInstance tempInstance{instance, solution};
            tspInstances->emplace_back(tempInstance, rawProblemName);
        }
    }

    closedir(dir);

    for (const auto& info : *tspInstances) {
        tspInstance instance = info.first;
        std::string problemName = info.second;
    }

    return tspInstances;
}

void runAllInstances(){

    listTSPInstances* listTspInstances = loadAllTSPInstances();
    int nExperiments = 20;
    int totalExperiments = listTspInstances->size() * nExperiments;
    int totalI = 1;

    if (listTspInstances == nullptr) throw std::runtime_error("No se pudo cargar las instancias");
    else {

        for (const auto& info : *listTspInstances) {
            tspInstance instance = info.first;
            std::string problemName = info.second;

            double avgTime = 0.0;
            double avgCost = 0.0;
            double avgIters = 0.0;
            double avgMaxIters = 0.0;

            double bestSolution = 9999999999999999.0;
            vector<typename GrafoP<double>::vertice> bestSolutionTour;
            for (int i = 0; i < nExperiments; ++i) {

                auto beg = high_resolution_clock::now();

                // Buscamos una solución
                Annealing<double> annealing = Annealing<double>(*instance.first, true);

                auto solution = annealing.findSolution();

                // Guardamos el tiempo que le ha tomado
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - beg);
                avgTime += duration.count();
                avgCost += solution.second;
                avgIters += annealing.iters();
                avgMaxIters += annealing.maxIters();
                totalI++;

                if (solution.second < bestSolution){
                    bestSolution = solution.second;
                    bestSolutionTour = solution.first;
                }
            }

            avgTime /= nExperiments;
            avgCost /= nExperiments;
            avgIters /= nExperiments;
            avgMaxIters /= nExperiments;

            auto optimalCost = evaluateOptimal(*instance.second, *instance.first);
            auto bestCost = evaluate(bestSolutionTour, *instance.first);

            std::cout << "----- Problema " << problemName << " -----" << std::endl;
            std::cout << "Coste solución óptima: " << optimalCost << std::endl;
            std::cout << "Coste mejor solución: " << bestCost << std::endl;
            std::cout << "Coste promedio solución encontrada: " << avgCost << std::endl;
            std::cout << "Error absoluto promedio: " << (avgCost - optimalCost) << std::endl;
            std::cout << "Error relativo promedio (fórmula): " << (avgCost / optimalCost * 100) << "%" << std::endl;
            std::cout << "Tiempo promedio necesitado (ms): " << avgTime << std::endl;
            std::cout << "Iteraciones realizadas: " << avgIters << std::endl;
            std::cout << "Iteraciones max permitidas: " << avgMaxIters << std::endl;
            std::cout << "------------------------------" << std::endl << std::endl;
        }
    }

    delete listTspInstances;
}

int main(){

    std::string PROBLEMS_PATH = "./problems/";
    std::string SOLUTIONS_PATH = "./solutions/";

    runAllInstances();

    return 0;
}