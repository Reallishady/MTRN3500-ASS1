#include "EmbeddedFunctions.h"

using namespace System;


int main()
{
    EmbeddedFunctions^ ef =
        gcnew EmbeddedFunctions();


    Console::WriteLine(
        "========================================"
    );

    Console::WriteLine(
        " EMBEDDED FUNCTIONS PHYSICAL TEST"
    );

    Console::WriteLine(
        "========================================"
    );


    // ============================================================
    // TEST 1: REAL TCP CONNECTION
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 1: GOpen(192.168.0.120, 23)"
        );

        ef->GOpen(
            "192.168.0.120",
            23
        );

        Console::WriteLine(
            "PASS: Physical TCP connection established"
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: Could not connect"
        );

        Console::WriteLine(
            "Exception: {0}",
            e->Message
        );

        Console::ReadLine();

        return 1;
    }


    // ============================================================
    // TEST 2: COMMAND WITH ;
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 2: QE 0;"
        );

        String^ response =
            ef->GCommand("QE 0;");

        Console::WriteLine(
            "Response: [{0}]",
            response
        );

        Console::WriteLine(
            "PASS if encoder value was returned"
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: {0}",
            e->Message
        );
    }


    // ============================================================
    // TEST 3: COMMAND WITHOUT ;
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 3: QE 0 without semicolon"
        );

        String^ response =
            ef->GCommand("QE 0");

        Console::WriteLine(
            "Response: [{0}]",
            response
        );

        Console::WriteLine(
            "PASS if encoder value was returned"
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: {0}",
            e->Message
        );
    }


    // ============================================================
    // TEST 4: MULTIPLE COMMANDS
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 4: Repeated commands"
        );

        for (int i = 0; i < 5; i++)
        {
            String^ response =
                ef->GCommand("QE 0");

            Console::WriteLine(
                "{0}: [{1}]",
                i + 1,
                response
            );
        }

        Console::WriteLine(
            "PASS: Connection remained usable"
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: {0}",
            e->Message
        );
    }


    // ============================================================
    // TEST 5: REAL WRITE
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 5: Physical digital write"
        );

        Console::WriteLine(
            "Turning DO0 ON..."
        );

        String^ response =
            ef->GCommand("OP 1,0");

        Console::WriteLine(
            "Response: [{0}]",
            response
        );

        Console::WriteLine(
            "Check DO0 physically turns ON."
        );

        Console::WriteLine(
            "Press ENTER to turn it back OFF."
        );

        Console::ReadLine();


        response =
            ef->GCommand("OP 0,0");

        Console::WriteLine(
            "Reset response: [{0}]",
            response
        );

        Console::WriteLine(
            "Check all digital outputs are OFF."
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: {0}",
            e->Message
        );
    }


    // ============================================================
    // TEST 6: CLOSE
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 6: GClose()"
        );

        ef->GClose();

        Console::WriteLine(
            "PASS: Connection closed cleanly"
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "FAIL: {0}",
            e->Message
        );
    }


    // ============================================================
    // TEST 7: COMMAND AFTER CLOSE
    // ============================================================

    try
    {
        Console::WriteLine(
            "\nTEST 7: Command after GClose"
        );

        String^ response =
            ef->GCommand("QE 0");

        Console::WriteLine(
            "FAIL: Command unexpectedly worked"
        );

        Console::WriteLine(
            "Response: [{0}]",
            response
        );
    }
    catch (Exception^ e)
    {
        Console::WriteLine(
            "PASS: Command after close rejected"
        );

        Console::WriteLine(
            "Exception: {0}",
            e->Message
        );
    }


    Console::WriteLine(
        "\n========================================"
    );

    Console::WriteLine(
        " PART B PHYSICAL TESTING COMPLETE"
    );

    Console::WriteLine(
        "========================================"
    );

    Console::ReadLine();

    return 0;
}