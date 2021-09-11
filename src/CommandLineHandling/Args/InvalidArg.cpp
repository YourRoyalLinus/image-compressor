#include "../../../include/CommandLineHandling/Args/InvalidArg.h"
#include <iostream>

InvalidArg::InvalidArg(char arg){
    optArg = arg;
    SetFlag();
}

void InvalidArg::Handle(){
    std::cout << "Invalid command line argument:" << optArg << std::endl;
    exit(-1);
}

void InvalidArg::SetFlag(){
    flag = '?';
}

