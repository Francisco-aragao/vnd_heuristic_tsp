#include <ctime>
#include <string>
#include "../include/utils.hpp"
#include <string>

using namespace std;
namespace fs = std::filesystem;

const int NUMBER_OF_ITERATIONS = 5;
Utils utils;

void vnd(ifstream& inputFile, bool useCenterCity) {

    double totalPathCost = 0;
    double totalElapsedTime = 0;
    
    std::clock_t start; // variable to control time management

    // collect results for 5 iterations and calculate the average
    //for (int iteration = 0; iteration < NUMBER_OF_ITERATIONS; ++iteration) {

        string distance_type;
        int numCities;        
    
        start = std::clock();

        vector<string> res(2);
        res = utils.findPathInfo(inputFile);

        numCities = stoi(res[0]);
        distance_type = res[1];

        int improved = true;
        double pathCost;

        pathCost = utils.constructive_heuristic(inputFile, numCities, distance_type, useCenterCity);
        
        while(improved == true) {

            improved = false;

            cout << "Initial Path Cost: " << pathCost << "\n";

            double newPathCost = utils.two_opt(utils.getCities());

            cout << "2 New Path Cost: " << newPathCost << "\n";

            if (newPathCost < pathCost) {
                pathCost = newPathCost;
                improved = true;
                continue;
            }

            newPathCost = utils.three_opt(utils.getCities());

            cout << "3 New Path Cost: " << newPathCost << "\n";

            if (newPathCost < pathCost) {
                pathCost = newPathCost;
                improved = true;
                continue;
            }
            newPathCost = utils.double_bridge(utils.getCities());

            cout << "Bridge New Path Cost: " << newPathCost << "\n";

            if (newPathCost < pathCost) {
                pathCost = newPathCost;
                improved = true;
                continue;
            }

        }
        

        double elapsed = double(std::clock() - start) / CLOCKS_PER_SEC;

    //}

    // calculate the average path cost and elapsed time for the instance

    vector<int> path = utils.getPath();

    cout << endl;
    cout << "Path Cost: " << pathCost << "\n";
    cout << "Elapsed Time: " << elapsed << " seconds\n";
    cout << "Path: ";
    for (int i = 0; i < (int) path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << endl;
    throw;
    return;
}

int main(int argc, char* argv[]) {

    // receive input folder path and type of city to start in the command line
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <folder_path>" << " <use_center_city>" << endl;
        cerr << "Example: " << argv[0] << " ./tsp_files 1 -> using center city" << endl;
        cerr << "Example: " << argv[0] << " ./tsp_files 0 -> using first city" << endl;
        return 1;
    }

    string folderPath = argv[1];
    bool useCenterCity = stoi(argv[2]);


    // Process all .tsp files in the folder
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.path().extension() == ".tsp") { // just open .tsp files

            string filename = entry.path().string();
            ifstream inputFile(filename);

            if (!inputFile.is_open()) {
                cerr << "Failed to open file: " << filename << endl;
                return 1;
            }

            cout << "Results for " << filename << ":\n";
            vnd(inputFile, useCenterCity);
        }
    }

    return 0;
}