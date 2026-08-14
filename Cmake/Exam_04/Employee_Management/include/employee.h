#pragma once
#include <string>

class Employee(){
    protected:
        std::string name ;
        int id;
        
    public:
        Employee (const std::string& name , int id );

        virtual ~Employee() = default ;

        virtual void displayInfo() const = 0 ;
        virtual double  calSalary() const = 0;
};
