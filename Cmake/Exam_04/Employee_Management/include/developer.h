#pragma once
#include "employee.h"

class Developer : public Employee {
    private:
        double baseSalary ;
    public:
        Developer(const std::string& name , int id , double baseSalary);
        
        void displayInfo() const override;
        double calculateSalary() const override; 
};


