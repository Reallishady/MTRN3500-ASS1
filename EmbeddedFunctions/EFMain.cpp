#include "EmbeddedFunctions.h"

using namespace System;


int main()
{
    EmbeddedFunctions^ ef =
        gcnew EmbeddedFunctions();

    EmbeddedFunctions^ ef2 =
        gcnew EmbeddedFunctions();

    EmbeddedFunctions^ ef3 =
        gcnew EmbeddedFunctions();
   ef->GOpen("192.168.0.120", 23);
    ////String^ response = ef->GCommand("OP ,255");
    //String^ response = ef->GCommand("CB 0");
   
   ef2->GOpen("192.168.0.120", 23);
    //String^ res2 = ef2->GCommand("CB 1");
    //Console::ReadLine();

   ef3->GOpen("192.168.0.120", 23);
    //String^ res3 = ef3->GCommand("CB 2");
    //EmbeddedFunctions^ ef4 =
    //    gcnew EmbeddedFunctions();
    //ef4->GOpen("192.168.0.120", 23);
    //String^ res4 = ef4->GCommand("CB 14");
    String^ res1 = ef->GCommand("OP 255,0;");
    System::Threading::Thread::Sleep(1000);
    String^ res2 = ef2->GCommand("AO 0, 2.5;");
    System::Threading::Thread::Sleep(1000);
    String^ res3 = ef3->GCommand("CB 0");
    return 0;
}