/**
 * This file is given and should not be changed
 */

#include <iostream>
#include <string.h>
#include "changroberts.hpp"
#include "franklin.hpp"

void printHelp()
{
    std::cout << "Usage: ./hwa5 COMMAND"
                 "\n\nCommands:\n"
                 "  changroberts     run the Chang and Roberts leader election algorithm\n"
                 "  franklin         run the Franklin leader election algorithm\n"
              << std::endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        // no argument was provided
        printHelp();
        return 1;
    }
    else if (argc == 2)
    {
        // check arguments
        if (strcmp(argv[1], "changroberts") != 0 && strcmp(argv[1], "franklin") != 0)
        {
            printHelp();
            return 1;
        }

        // argument valid, start the algorithm given by COMMAND
        if (strcmp(argv[1], "changroberts") == 0)
        {
            ChangRoberts().run(argc, argv);
        }
        else
        {
            Franklin().run(argc, argv);
        }
    }
    else
    {
        printHelp();
        return 1;
    }
}
