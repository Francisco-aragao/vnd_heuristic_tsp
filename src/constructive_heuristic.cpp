#include <ctime>
#include <string>
#include "../include/utils.hpp"
#include <string>

using namespace std;
namespace fs = std::filesystem;

const int NUMBER_OF_ITERATIONS = 5;
Utils utils;

void processFile(const string& filename, bool useCenterCity) {

    double totalPathCost = 0;
    double totalElapsedTime = 0;
    
    std::clock_t start; // variable to control time management

    // collect results for 5 iterations and calculate the average
    for (int iteration = 0; iteration < NUMBER_OF_ITERATIONS; ++iteration) {

        string distance_type;
        int numCities;
        vector<City> cities;
        ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        start = std::clock();

        vector<string> res(2);
        res = utils.findPathInfo(inputFile);

        numCities = stoi(res[0]);
        distance_type = res[1];

        cities = utils.receiveCoordinatesParameters(inputFile, numCities, distance_type);

        int initialCityId = 1;
        if ( useCenterCity )
            initialCityId = utils.findCenterCity(cities, numCities);

        double pathCost = utils.findPath(initialCityId, cities, numCities);

        double elapsed = double(std::clock() - start) / CLOCKS_PER_SEC;

        totalPathCost += pathCost;
        totalElapsedTime += elapsed;
    }

    // calculate the average path cost and elapsed time for the instance

    double avgPathCost = totalPathCost / NUMBER_OF_ITERATIONS;
    double avgElapsedTime = totalElapsedTime / NUMBER_OF_ITERATIONS;

    vector<int> path = utils.getPath();

    cout << endl;
    cout << "Results for " << filename << ":\n";
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
            processFile(entry.path().string(), useCenterCity);
        }
    }

    return 0;
}