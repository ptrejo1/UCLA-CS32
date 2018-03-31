//
//  stu.h
//  3file
//
//  Created by Phoenix Trejo on 2/3/18.
//  Copyright © 2018 Phoenix Trejo. All rights reserved.
//

#ifndef stu_h
#define stu_h

#include <string>

class Student{
private:
    int age;
    std::string name;
    
public:
    Student();
    Student(int age, std::string name);
    
    int getAge();
    
};

#endif 
/* stu_h */
