#pragma once

#include "Component.hpp"

#include <iostream>

int main()
{
    ::std::string input = {};

    ::std::cout << "Starting the linux machine" << ::std::endl;

    while ( true )
    {
        ::std::cout << "Type 'n' or 'stop' to exit the machine, type anything to continue" << ::std::endl;
        ::std::cin >> input;
        if ( input == "n" || input == "stop" )
        {
            break;
        }

        EngineLoop();
    }

    ::std::cout << "Goodbye machine" << ::std::endl;
}
