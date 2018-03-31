#include <iostream>
#include "stu.h"
#include <string>

using namespace std;

struct Node{
    string name;
    Node *next;
};

class LinkedList{
public:
    LinkedList(){
        head = nullptr;
    }
    
    void printItems(){
        Node *p;
        p = head;
        
        while (p != nullptr) {
            cout << p->name << endl;
            p = p->next;
        }
    }
    
    void addToFront(string v){
        Node *p;
        p = new Node;
        p->name = v;
        p->next = head;
        head = p;
    }
    
    void addToRear(string v){
        if (head == nullptr) {
            addToRear(v);
        } else {
            Node *p;
            p = head;
            
            while (p->next != nullptr) {
                p = p->next;
            }
            
            Node *n = new Node;
            n->name = v;
            p->next = n;
            n->next = nullptr;
        }
    }
    
    void addItem(string newItem){
        if (head == nullptr) {
            addToFront(newItem);
        } else if (newItem < head->name){
            addToFront(newItem);
        } else {
            Node *p = head;
            
            while (p->next != nullptr) {
                if (newItem >= p->name && newItem <= p->next->name) {
                    break;
                }
                p = p->next;
            }
            
            Node *latest = new Node;
            latest->name = newItem;
            latest->next = p->next;
            p->next = latest;
        }
    }
    
    void deleteItem(string v){
        if(head == nullptr){
            return;
        }
        
        if (head->name == v) {
            Node *killMe = head;
            head = killMe->next;
            delete killMe;
            return;
        }
        
        // Next part of delete
        Node *p = head;
        while(p != nullptr){
            if(p->next != nullptr && p->next->name == v){
                break;
            }
            p = p->next;
        }
        
        if(p != nullptr){
            Node *killMe = p->next;
            p->next = killMe->next;
            delete killMe;
        }
    }
    
    bool findItem(string v){
        if(head->name == v){
            return true;
        }
        
        Node *p = head;
        while(p != nullptr){
            if(p->name == v){
                return true;
            } else {
                p = p->next;
            }
        }
        
        return false;
    }
    
    ~LinkedList(){
        Node *p;
        p = head;
        while(p != nullptr){
            Node *n = p->next;
            delete p;
            p = n;
        }
    }
    
    
private:
    Node *head;
    Node *tail;
    
};



int main() {
    
    return 0;
}
