#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;

// Remove the odd integers from li.
// It is acceptable if the order of the remaining even integers is not
// the same as in the original list.
void removeOdds(list<int>& li) {
    
    list<int>::iterator it;
    for(it = li.begin(); it != li.end(); it++){
        if (*it % 2 != 0) {
            li.erase(it);
        }
    }
}

// Specfic problem here due to nature of vector
void removeOdds(vector<int>& v) {
    
    vector<int>::iterator it;
    for (it = v.begin(); it != v.end();) {
        if (*it % 2 != 0) {
            v.erase(it);
        } else {
            it++;
        }
    }
    
    /* assume all the addresses change not true tho
    for(int i = 0; i < v.size(); i++){
        vector<int>::iterator it;
        it = v.begin()+i;
        
        if (*it % 2 != 0) {
            v.erase(it);
            i--;
        }
    }
     */
}

vector<int> destroyedOnes;

class Movie
{
public:
    Movie(int r) : m_rating(r) {}
    ~Movie() { destroyedOnes.push_back(m_rating); }
    int rating() const { return m_rating; }
private:
    int m_rating;
};

// Remove the movies in li with a rating below 50 and destroy them.
// It is acceptable if the order of the remaining movies is not
// the same as in the original list.
void removeBad(list<Movie*>& li){
    
    list<Movie*>::iterator it;
    for (it = li.begin(); it != li.end(); it++) {
        if ((*it)->rating() < 50) {
            delete (*it);
            li.erase(it);
        }
    }
}

const int MAGIC = 11223344;

void test()
{
    bool allValid = true;
    
    vector<int> v1(5, MAGIC);
    int k = 0;
    for ( ; k != v1.size(); k++)
    {
        if (v1[k] != MAGIC)
        {
            cout << "v1[" << k << "] is " << v1[k] << ", not " << MAGIC <<"!" << endl;
            allValid = false;
        }
        if (k == 2)
        {
            for (int i = 0; i < 5; i++)
                v1.push_back(MAGIC);
        }
    }
    if (allValid  &&  k == 10)
        cout << "Passed test 1" << endl;
    else
        cout << "Failed test 1" << endl;
    
    allValid = true;
    list<int> l1(5, MAGIC);
    k = 0;
    for (list<int>::iterator p = l1.begin(); p != l1.end(); p++, k++)
    {
        if (*p != MAGIC)
        {
            cout << "Item# " << k << " is " << *p << ", not " << MAGIC <<"!" << endl;
            allValid = false;
        }
        if (k == 2)
        {
            for (int i = 0; i < 5; i++)
                l1.push_back(MAGIC);
        }
    }
    if (allValid  &&  k == 10)
        cout << "Passed test 2" << endl;
    else
        cout << "Failed test 2" << endl;
    
    /**
     Test 3 fails for the same reason found in problem 2
     pushing an item back shuffles the memory and the iterator
     will now point to the wrong memory address. It worked 
     fine in the t1 and t2 because we used for loop or list
    */
    allValid = true;
    vector<int> v2(5, MAGIC);
    k = 0;
    for (vector<int>::iterator p = v2.begin(); p != v2.end(); p++, k++)
    {
        if (k >= 20)  // prevent infinite loop
            break;
        if (*p != MAGIC)
        {
            cout << "Item# " << k << " is " << *p << ", not " << MAGIC <<"!" << endl;
            allValid = false;
        }
        if (k == 2)
        {
            for (int i = 0; i < 5; i++)
                v2.push_back(MAGIC);
        }
    }
    if (allValid  &&  k == 10)
        cout << "Passed test 3" << endl;
    else
        cout << "Failed test 3" << endl;
}

int main(){
    
    vector<int*> a;
    
    a.push_back(new int(3));
    a.push_back(new int(4));
    a.push_back(new int(5));
    
    vector<int*>::iterator it;
    
    for (it = a.begin(); it != a.end();) {
        
        if (**it == 4) {
            delete *it;
            it = a.erase(it);
        } else {
            it++;
        }
        
        
    }
    

    
}

