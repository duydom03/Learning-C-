#include <iostream>
#include <fstream>

int main()
{
    std::ofstream file("data.txt");

    if (!file.is_open())
    {
        std::cout<<"No opened file! \n";
        return 1;
    }
    
    std::cout<<"Success open file!\n";

    file << "Hello baby !\n";
    file << "File I/O. \n";
    file << 100 <<"\n";

    file.close();

    return 0;
}