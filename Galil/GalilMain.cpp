#include "Galil.h"
#include <iostream>
#using<System.dll>
int main()
{
    EmbeddedFunctions* funcs = new EmbeddedFunctions();
    Galil myGalil(funcs, "192.168.0.120 -d");

    //while (true) {
    //    myGalil.AnalogOutput(0, 2.0);
    //    System::Threading::Thread::Sleep(1000);
    //    myGalil.AnalogOutput(0, -2.0);
    //    System::Threading::Thread::Sleep(1000);
    //}
    std::cout << "Enter to start";
    System::Console::ReadKey();

    double voltage = 1;
    while (true) {
        double s = myGalil.AnalogInput(0);
        myGalil.AnalogOutput(0,s);
    }
    



    return 0;
}
