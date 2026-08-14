#include <iostream>
#include <fstream>

int main()
{
    std::ifstream file("data.txt");

    if (!file.is_open())
    {
        std::cout<<"No opened file!\n";
        return 1;
    }

    std::string line ;

    while (std::getline(file , line))
    {
        std::cout<<line<<"\n";
    } 

    return 0;
}