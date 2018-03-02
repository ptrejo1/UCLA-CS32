#include "Map.h"
#include <string>

bool combine(const Map& m1, const Map& m2, Map& result){
    
    result = m1;
    
    bool combined = true;
    for (int i = 0; i < m2.size(); i++) {
        KeyType k;
        ValueType v;
        m2.get(i, k, v);
        
        // if not able to insert check for value discrepency
        if (!result.insert(k, v)) {
            ValueType v2;
            result.get(k, v2);
            
            if (v != v2) {
                result.erase(k);
                combined = false;
            }
        }
    }
    return combined;
}

void subtract(const Map& m1, const Map& m2, Map& result){
    
    result = m1;
    
    // erase keys in result found in m2
    for (int i = 0; i < m2.size(); i++) {
        KeyType k;
        ValueType v;
        m2.get(i, k, v);
        result.erase(k);
    }
}

Map::Map() : counter(0){
    head = nullptr;
    tail = nullptr;
}

// Copy Constructor
// Dont use for Map with no items
Map::Map(const Map& other) {
    
    counter = 0;
    head = nullptr;
    tail = nullptr;
    
    for (int i = 0; i < other.counter; i++) {
        KeyType k;
        ValueType v;
        other.get(i, k, v);
        this->insert(k, v);
    }
}

Map::~Map(){
    
    Node *p = head;
    while (p != nullptr) {
        Node *n = p;
        p = p->next;
        delete n;
    }    
}

Map& Map::operator=(const Map &other){
    
    // free old nodes
    Node *p = head;
    while (p != nullptr) {
        Node *n = p;
        p = p->next;
        delete n;
    }
    
    // reset memeber variables
    counter = 0;
    head = nullptr;
    tail = nullptr;
    
    // insert other map values
    for (int i = 0; i < other.counter; i++) {
        KeyType k;
        ValueType v;
        other.get(i, k, v);
        this->insert(k, v);
    }

    return (*this);
}

bool Map::empty() const{
    if (counter == 0) {
        return true;
    } else {
        return false;
    }
}

int Map::size() const{
    return counter;
}

bool Map::insert(const KeyType &key, const ValueType &value){
    
    // check if empty
    if(head == nullptr){
        Node *p;
        p = new Node;
        p->key = key;
        p->value = value;
        p->next = nullptr;
        p->prev = nullptr;
        head = p;
        tail = p;
        counter++;
        return true;
    }
    
    Node *n = head;
    while (n != nullptr) {
        if (n->key == key) {
            return false;
        }
        n = n->next;
    }
    
    Node *p = new Node;
    p->key = key;
    p->value = value;
    p->prev = tail;
    p->next = nullptr;
    tail->next = p;
    tail = p;
    counter++;
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value){
    
    Node *p = head;
    while (p != nullptr) {
        if (p->key == key) {
            p->value = value;
            return true;
        }
        p = p->next;
    }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value){
    
    // if update fails then insert key
    if (!update(key, value)) {
        if(head == nullptr){
            Node *p;
            p = new Node;
            p->key = key;
            p->value = value;
            p->next = head;
            head = p;
            tail = p;
            counter++;
            return true;
        }
        
        Node *p = new Node;
        p->key = key;
        p->value = value;
        p->prev = tail;
        p->next = nullptr;
        tail->next = p;
        tail = p;
        counter++;
    }
    
    return true;
}

bool Map::erase(const KeyType &key){
    
    // empty
    if (head == nullptr) {
        return true;
    }
    
    // delete front
    if (head->key == key) {
        if (counter == 1) {
            Node *kill = head;
            head = nullptr;
            tail = nullptr;
            counter--;
            delete kill;
        }
        
        Node *killMe = head;
        head = killMe->next;
        head->prev = nullptr;
        delete killMe;
        counter--;
        return true;
    }
    
    // delete middle
    Node *p = head;
    while (p->next != nullptr) {
        if (p->key == key) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            delete p;
            counter--;
            return true;
        }
        p = p->next;
    }
    
    // delete last
    if (p->key == key) {
        tail = p->prev;
        p->prev->next =nullptr;
        delete p;
        counter--;
        return true;
    }
    
    return false;
}

bool Map::contains(const KeyType &key) const {
    
    Node *p = head;
    while (p != nullptr) {
        if (p->key == key) {
            return true;
        }
        p = p->next;
    }
    
    return false;
}

bool Map::get(const KeyType &key, ValueType &value) const {
    
    Node *p = head;
    while (p != nullptr) {
        if (p->key == key) {
            value = p->value;
            return true;
        }
        p = p->next;
    }
    
    return false;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    
    Node *p = head;
    if (i >= 0 && i < counter) {
        int j = 0;
        while (p != nullptr) {
            if (j == i) {
                key = p->key;
                value = p->value;
                return true;
            }
            
            p = p->next;
            j++;
        }
    }
    
    return false;
}

void Map::swap(Map &other){
    Node *temp = head;
    head = other.head;
    other.head = temp;
    
    temp = tail;
    tail = other.tail;
    other.tail = temp;
    
    int hold = counter;
    counter = other.counter;
    other.counter = hold;
}