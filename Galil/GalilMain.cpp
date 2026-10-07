#include "Galil.h"
#include <iostream>

int main()
{
    EmbeddedFunctions funcs(true);
    Galil myGalil(&funcs, "192.168.0.120 -d");

    std::cout << "Setting DO0 ON..." << std::endl;
    myGalil.DigitalOutput(1);

    std::cout << "Press Enter..." << std::endl;
    std::cin.get();

    std::cout << "Setting DO0-DO7 ON..." << std::endl;
    myGalil.DigitalOutput(255);

    std::cout << "Press Enter to finish..." << std::endl;
    std::cin.get();

    return 0;
}