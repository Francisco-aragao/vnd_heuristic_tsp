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
    
    vector<string> res(2);
    res = utils.findPathInfo(inputFile);

    int numCities = stoi(res[0]);
    string distance_type = res[1];

    std::clock_t start = std::clock(); // variable to control time management

    double pathCost = utils.constructive_heuristic(inputFile, numCities, distance_type, useCenterCity);
    
    double firstSolutionTime = double(std::clock() - start) / CLOCKS_PER_SEC;

    cout << "First Solution Time: " << firstSolutionTime << " seconds\n";

    vector<City> originalCities = utils.getCities();
    vector<int> originalPath = utils.getPath();
    double originalPathCost = pathCost;

    // collect results for N iterations and calculate the average
    for (int iteration = 0; iteration < NUMBER_OF_ITERATIONS; iteration++) {

        start = std::clock();    

        int improved = true;

        utils.setCities(originalCities);
        utils.setPath(originalPath);
        utils.setPathCost(originalPathCost);
        
        cout << "Initial Path Cost: " << pathCost << "\n";
        
        while(improved == true) {

            improved = false;

            pathCost = utils.getPathCost();


            double newPathCost = utils.two_opt(utils.getCities());

            if (newPathCost < pathCost) { // restart the loop if the path cost is improved
                pathCost = newPathCost;
                improved = true;
                continue;
            }

            newPathCost = utils.three_opt(utils.getCities());


            if (newPathCost < pathCost) {
                pathCost = newPathCost;
                improved = true;
                continue;
            }

            newPathCost = utils.double_bridge(utils.getCities());

            if (newPathCost < pathCost) {
                pathCost = newPathCost;
                improved = true;
                continue;
            }

        }
        
        double VNDTime = double(std::clock() - start) / CLOCKS_PER_SEC;

        totalPathCost += pathCost;
        totalElapsedTime += VNDTime;

        cout << "VND Time: " << VNDTime << " seconds\n";
    }

    // calculate the average path cost and elapsed time for the instance

    double avgPathCost = totalPathCost / NUMBER_OF_ITERATIONS;
    double avgElapsedTime = totalElapsedTime / NUMBER_OF_ITERATIONS;

    vector<int> path = utils.getPath();

    cout << "Average Path Cost: " << avgPathCost << "\n";
    cout << "Average Elapsed Time: " << avgElapsedTime << " seconds\n";
    cout << "Path: ";
    for (int i = 0; i < (int) path.size(); i++) {
        cout << path[i] << " ";
    }
    cout << endl;
    cout << endl;


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