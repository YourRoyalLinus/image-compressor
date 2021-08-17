#include "../include/HelpArg.h"
#include <iostream>


HelpArg::HelpArg(){
    SetFlag();
}

void HelpArg::Handle(){
    std::cout << helpMsg;
    exit(0);
}

void HelpArg::SetFlag(){
    flag = 'h';
}