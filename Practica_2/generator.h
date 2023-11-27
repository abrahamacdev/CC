#include <vector>
#include "types.h"

instance& singleGenerator(int k, int nStudents);
experiment* batchGenerator(int initialN, int lastN, int step, int nStudents);

void saveExperiment(experiment& experiment, const std::string& dir = std::string ("./"));