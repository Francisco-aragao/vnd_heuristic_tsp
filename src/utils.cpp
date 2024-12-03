#include "../include/utils.hpp"

double Utils::findPath(int initialCityId, vector<City> cities, int numCities) {
    vector<int> path;

    path.push_back(initialCityId);

    int currentCityId = initialCityId;
    
    double pathCost = 0;

    while ((int)path.size() < numCities) {
        double minDistance = 5000000;
        int nextCityId = 0;

        for (auto city : cities) {
            if (city.getId() == 0)
                continue;

            if (city.getId() == currentCityId)
                continue;

            if (find(path.begin(), path.end(), city.getId()) != path.end())
                continue;

            double distance = city.returnDistanceTo(currentCityId);
            if (distance < minDistance) {
                if (minDistance != 5000000)
                    pathCost -= minDistance;

                pathCost += distance;
                minDistance = distance;
                nextCityId = city.getId();
            }
        }

        path.push_back(nextCityId);
        currentCityId = nextCityId;
    }
    
    path.push_back(initialCityId);
    pathCost += cities[currentCityId].returnDistanceTo(initialCityId);

    this->path = path;

    return pathCost;
}

int Utils::findCenterCity(vector<City> cities, int numCities) {
    double minDistance = 5000000;
    int idCenterCity = 0;
    for (auto city : cities) {
        if (city.getId() == 0)
            continue;

        double sum = 0;
        for (int i = 1; i <= numCities; i++) {
            if (city.getId() == i)
                continue;

            sum += city.returnDistanceTo(i);
        }

        if (sum < minDistance) {
            minDistance = sum;
            idCenterCity = city.getId();
        }
    }

    return idCenterCity;
}

vector<City> Utils::receiveCoordinatesParameters(ifstream& inputFile, int numCities, string DISTANCE_TYPE) {
    
    std::vector<City> cities;

    City initialCity(0, 0, 0, numCities);
    cities.push_back(initialCity);
    
    int id;
    float x, y;
    for (int i = 0; i < numCities; i++) {
        inputFile >> id >> x >> y;
        City newCity(id, x, y, numCities);
        cities.push_back(newCity);

        for (auto city : cities) {
            if (city.getId() == 0)
                continue;

            if (city == newCity)
                continue;

            if (city.returnDistanceTo(id) != -1)
                continue;

            double distance = city.calculateDistanceTo(newCity, DISTANCE_TYPE);
            city.setDistances(id, distance);
            newCity.setDistances(city.getId(), distance);

            cities[city.getId()] = city;
            cities[id] = newCity;
        }
    }

    return cities;
}

vector<string> Utils::findPathInfo(ifstream& inputFile) {

    string info;

    string numCities;
    string distance_type;

    while (info != "NODE_COORD_SECTION") {

        inputFile >> info;
        
        if (info == "DIMENSION:") {
            inputFile >> info;

            numCities =info;
        } else if (info == "DIMENSION") {
            inputFile >> info; // reading ':'
            inputFile >> info;
            numCities = info;
        }

        if (info == "EDGE_WEIGHT_TYPE:") {
            inputFile >> info;
            distance_type = info;
        } else if (info == "EDGE_WEIGHT_TYPE") {
            inputFile >> info; // reading ':'
            inputFile >> info;
            distance_type = info;
        }
    }


    vector<string> results;
    results.push_back(numCities);
    results.push_back(distance_type);

    return results;
}

double Utils::constructive_heuristic(ifstream& inputFile, int numCities, string distance_type, bool useCenterCity) {
    vector<City> cities;
    cities = this->receiveCoordinatesParameters(inputFile, numCities, distance_type);

    int initialCityId = 1;
    if ( useCenterCity )
        initialCityId = this->findCenterCity(cities, numCities);

    double pathCost = this->findPath(initialCityId, cities, numCities);
}