#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <fstream>
#include <filesystem>
#include "city.hpp"

using namespace std;

// class to handle utility functions for reading and processing input data, and calculating the path

class Utils {
    private:
        vector<int> path;
    public:        
        vector<int> getPath() {return path;};
        double findPath(int initialCityId, vector<City> cities, int numCities);
        int findCenterCity(vector<City> cities, int numCities);
        vector<City> receiveCoordinatesParameters(ifstream& inputFile, int numCities, string DISTANCE_TYPE);
        vector<string> findPathInfo(ifstream& inputFile);

};

#endif
