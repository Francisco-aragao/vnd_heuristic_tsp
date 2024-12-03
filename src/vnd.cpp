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
    for (int iteration = 0; iteration < NUMBER_OF_ITERATIONS; ++iteration) {

        string distance_type;
        int numCities;        
    
        start = std::clock();

        vector<string> res(2);
        res = utils.findPathInfo(inputFile);

        numCities = stoi(res[0]);
        distance_type = res[1];

        int tries = 10;

        for (; tries >= 0; tries--) {
            double pathCost = utils.constructive_heuristic(inputFile, numCities, distance_type, useCenterCity);

            // CHAMAR O 2-OPT AQUI COM O PATH ATUAL
            // CHAMAR O 3-OPT AQUI COM O NOVO PATH
        }
        

        double elapsed = double(std::clock() - start) / CLOCKS_PER_SEC;

        totalPathCost += pathCost;
        totalElapsedTime += elapsed;
    }

    // calculate the average path cost and elapsed time for the instance

    double avgPathCost = totalPathCost / NUMBER_OF_ITERATIONS;
    double avgElapsedTime = totalElapsedTime / NUMBER_OF_ITERATIONS;

    vector<int> path = utils.getPath();

    cout << endl;
    cout << "Average Path Cost: " << avgPathCost << "\n";
    cout << "Average Elapsed Time: " << avgElapsedTime << " seconds\n";
    cout << "Path: ";
    for (int i = 0; i < (int) path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << endl;
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
                return;
            }

            cout << "Results for " << filename << ":\n";
            vnd(inputFile, useCenterCity);
        }
    }

    return 0;
}