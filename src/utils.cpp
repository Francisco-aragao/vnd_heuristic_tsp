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

    this->cities = cities;

    int initialCityId = 1;
    if ( useCenterCity )
        initialCityId = this->findCenterCity(cities, numCities);

    double pathCost = this->findPath(initialCityId, cities, numCities);

    this->pathCost = pathCost;

    return pathCost;
}

double Utils::two_opt(vector<City> cities) {

    vector<int> initialPath = this->path;

    double newPathCost = this->pathCost + 1;
    vector<int> newPath;

    int tries = 0;
    while (newPathCost > this->pathCost) {
        tries++;

        int randomPos1 = rand() % (int)initialPath.size();
        int randomPos2 = rand() % (int)initialPath.size();

        while (randomPos1 == randomPos2) {
            randomPos2 = rand() % (int)initialPath.size();
        }

        newPath = initialPath;

        int temp = newPath[randomPos1];
        newPath[randomPos1] = newPath[randomPos2];
        newPath[randomPos2] = temp;

        newPathCost = 0;
        for (int i = 0; i < (int)newPath.size() - 1; i++) {
            newPathCost += cities[newPath[i]].returnDistanceTo(newPath[i + 1]);
        }

        newPathCost += cities[newPath[newPath.size() - 1]].returnDistanceTo(newPath[0]);

    }
    cout << "Tries: " << tries << endl;


    //
    /* tries = 0; 
    newPathCost = this->pathCost + 1;
    newPath = initialPath;

    bool improved = false;

    for (int i = 0; i < (int)initialPath.size() && !improved ; i++) {

        for (int j = 0; j < (int) initialPath.size() && !improved; j++) {

            if (i == j)
                continue;

            tries++;
            newPath = initialPath;

            int temp = newPath[i];
            newPath[i] = newPath[j];
            newPath[j] = temp;

            newPathCost = 0;
            for (int i = 0; i < (int)newPath.size() - 1; i++) {
                newPathCost += cities[newPath[i]].returnDistanceTo(newPath[i + 1]);
            }

            newPathCost += cities[newPath[newPath.size() - 1]].returnDistanceTo(newPath[0]);

            if (newPathCost < this->pathCost) {
                improved = true;
            }
        }   
    }
    cout << "Tries: " << tries << endl; */
    //

    this->path = newPath;
    this->pathCost = newPathCost;

    return newPathCost;

}

double Utils::three_opt(vector<City> cities) {
    
        vector<int> initialPath = this->path;

    double newPathCost = this->pathCost + 1;
    vector<int> newPath;

    int tries = 0;
    while (newPathCost > this->pathCost) {
        tries++;

        int randomPos1 = rand() % (int)initialPath.size();
        int randomPos2 = rand() % (int)initialPath.size();
        int randomPos3 = rand() % (int)initialPath.size();

        while (randomPos1 == randomPos2 || randomPos1 == randomPos3 || randomPos2 == randomPos3) {
            randomPos2 = rand() % (int)initialPath.size();
            randomPos3 = rand() % (int)initialPath.size();
        }

        newPath = initialPath;

        int temp = newPath[randomPos1];
        newPath[randomPos1] = newPath[randomPos2];
        newPath[randomPos2] = newPath[randomPos3];
        newPath[randomPos3] = temp;

        newPathCost = 0;
        for (int i = 0; i < (int)newPath.size() - 1; i++) {
            newPathCost += cities[newPath[i]].returnDistanceTo(newPath[i + 1]);
        }

        newPathCost += cities[newPath[newPath.size() - 1]].returnDistanceTo(newPath[0]);

    }

    /* newPathCost = this->pathCost + 1;
    newPath = initialPath;

    bool improved = false;

    for (int i = 0; i < (int)initialPath.size() && !improved ; i++) {

        for (int j = 0; j < (int) initialPath.size() && !improved; j++) {
            
            for (int k = 0; k < (int) initialPath.size() && !improved; k++) {

                if (i == j || j == k || i == k)
                    continue;

                tries++;
                newPath = initialPath;

                int temp = newPath[i];
                newPath[i] = newPath[j];
                newPath[i] = newPath[k];
                newPath[k] = temp;

                newPathCost = 0;
                for (int i = 0; i < (int)newPath.size() - 1; i++) {
                    newPathCost += cities[newPath[i]].returnDistanceTo(newPath[i + 1]);
                }

                newPathCost += cities[newPath[newPath.size() - 1]].returnDistanceTo(newPath[0]);

                if (newPathCost < this->pathCost) {
                    improved = true;
                }
            }
        }   
    } */
    cout << "Tries: " << tries << endl;

    this->path = newPath;
    this->pathCost = newPathCost;

    return newPathCost;
}