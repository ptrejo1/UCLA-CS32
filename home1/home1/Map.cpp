
#include "Map.h"
#include <string>

Map::Map() : counter(0) {}

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
    
    if (counter != DEFAULT_MAX_ITEMS) {
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
    
    if (counter != DEFAULT_MAX_ITEMS) {
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
    
    int min = counter > other.counter ? other.counter : counter;
    
    for (int i = 0; i < min; i++) {
        m_tuple temp = dict[i];
        dict[i] = other.dict[i];
        other.dict[i] = temp;
    }
    
    if (counter > min) {
        for (int i = min; i < counter; i++) {
            other.dict[i] = dict[i];
        }
    } else if (other.counter > min) {
        for (int i = min; i < other.counter; i++) {
            dict[i] = other.dict[i];
        }
    }
    
    int temp = counter;
    counter = other.counter;
    other.counter = temp;
}
