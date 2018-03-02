
#include "CarMap.h"
#include "newMap.h"
#include <string>
#include <iostream>

CarMap::CarMap() {}

bool CarMap::addCar(std::string license){
    return m_map.insert(license, 0);
}

double CarMap::gas(std::string license) const {
    
    double gas;
    if (m_map.get(license, gas)) {
        return gas;
    }
    
    return -1;
}

bool CarMap::addGas(std::string license, double gallons){
    
    if (gallons < 0) {
        return false;
    }
    
    double gas;
    if (m_map.get(license, gas)) {
        gas += gallons;
        m_map.update(license, gas);
        return true;
    }
    
    return false;
}

bool CarMap::useGas(std::string license, double gallons){
    
    if (gallons < 0 || gallons > gas(license)) {
        return false;
    }
    
    double gas;
    if (m_map.get(license, gas)) {
        gas -= gallons;
        m_map.update(license, gas);
        return true;
    }
    
    return false;
}

int CarMap::fleetSize() const {
    return m_map.size();
}

void CarMap::print() const {
    
    if (m_map.empty()) {
        std::cout << "Map is empty" << std::endl;
        return;
    }
    
    std::string license;
    double gas;
    for (int i = 0; i < fleetSize(); i++) {
        m_map.get(i, license, gas);
        std::cout << license << ":" << gas << std::endl;
    }
}
