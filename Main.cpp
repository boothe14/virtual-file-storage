#include <iostream>
#include "FileSystem.hpp"

bool running = true;

int main()
{
    FileSystem fs;
    Directory& root = fs.getCurrentDirectory();

    std::string input;

    while(running)
    {
        std::cout<<"[fs] "<<fs.getCurrentDirectory().getName()<<" >> ";
        std::getline(std::cin, input);
        parseInput(fs, input, root);
    }

    return 0;
}