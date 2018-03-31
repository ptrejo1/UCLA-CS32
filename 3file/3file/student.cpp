#include "stu.h"

Student::Student(){};
Student::Student(int age, std::string name){
    this->age = age;
    this->name = name;
}

int Student::getAge(){
    return this->age;
}



