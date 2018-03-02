
#include "newMap.h"
#include <string>
#include <iostream>
#include <cstdlib>

Map::Map() : counter(0), amount(DEFAULT_MAX_ITEMS) {
    dict = new m_tuple[DEFAULT_MAX_ITEMS];
}

Map::Map(int items) : counter(0), amount(items) {
    if (items < 0)
    {
        std::cout << "A Map capacity must not be negative." << std::endl;
        exit(1);
    }
    dict = new m_tuple[items];
}

Map::Map(const Map &other){
    this->counter = other.counter;
    this->amount = other.amount;
    dict = new m_tuple[amount];
    
    for (int i = 0; i < other.counter; i++) {
        dict[i] = other.dict[i];
    }
}

Map::~Map(){
    delete []dict;
}

Map& Map::operator=(const Map &other){
    
    delete []dict;
    this->counter = other.counter;
    this->amount = other.amount;
    dict = new m_tuple[amount];
    
    for (int i = 0; i < other.counter; i++) {
        dict[i] = other.dict[i];
    }
    
    return (*this);
}

bool Map::empty() const {
    
    if(counter == 0)
        return true;
    else
        return false;
}

int Map::size() const {
    return counter;
}

bool Map::insert(const KeyType &key, const ValueType &value){
    
    if (counter == 0) {
        dict[0].key = key;
        dict[0].value = value;
        counter++;
        return true;
    }
    
    if (counter != amount) {
        for (int i = 0; i < counter; i++) {
            if (dict[i].key == key)
                return false;
        }
        
        dict[counter].key = key;
        dict[counter].value = value;
        counter++;
        return true;
        
    } else {
        return false;
    }
}

bool Map::update(const KeyType &key, const ValueType &value){
    
    for (int i = 0; i < counter; i++) {
        if (dict[i].key == key){
            dict[i].value = value;
            return true;
        }
    }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value){
    
    if (counter == 0) {
        dict[0].key = key;
        dict[0].value = value;
        counter++;
        return true;
    }
    
    for (int i = 0; i < counter; i++) {
        if (dict[i].key == key){
            dict[i].value = value;
            return true;
        }
    }
    
    if (counter != amount) {
        dict[counter].key = key;
        dict[counter].value = value;
        counter++;
        return true;
        
    } else {
        return false;
    }
}

bool Map::erase(const KeyType &key){
    
    for (int i = 0; i < counter; i++) {
        if (dict[i].key == key){
            m_tuple temp = dict[i];
            dict[i] = dict[counter-1];
            dict[counter-1] = temp;
            counter--;
            return true;
        }
    }
    
    return false;
}

bool Map::contains(const KeyType &key) const {
    
    for (int i = 0; i < counter; i++) {
        if (dict[i].key == key){
            return true;
        }
    }
    
    return false;
}

bool Map::get(const KeyType &key, ValueType &value) const {
    
    for (int i = 0; i < counter; i++) {
        if (dict[i].key == key){
            value = dict[i].value;
            return true;
        }
    }
    
    return false;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    
    if (i >= 0 && i < counter) {
        key = dict[i].key;
        value = dict[i].value;
        return true;
    }
    
    return false;
}

void Map::swap(Map &other){
    
    m_tuple *data = dict;
    dict = other.dict;
    other.dict = data;
    
    int temp = counter;
    counter = other.counter;
    other.counter = temp;
    
    int cap = amount;
    amount = other.amount;
    other.amount = cap;
    
}
