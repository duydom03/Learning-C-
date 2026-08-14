#include <iostream>
#include "developer.h"

Developer::Developer(const std::string& name , int id , double baseSalary) : Employee(name ,id) , baseSalary(baseSalary){}

