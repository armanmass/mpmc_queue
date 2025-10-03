import std;
#include <MPMC.hpp>

int main()
{
    MPMC<int, 1024> q;        
    std::cout << "USING IMPORT STD" << std::endl;
    return 0;
}