#include "../../../include/CommandLineHandling/Args/NullArg.h"
#include "../../../include/Utils/Utils.h"
#include <iostream>

NullArg::NullArg(){
    SetFlag();
}

void NullArg::Handle(){
    std::string input;
    Artifact art;

    std::cout << "Enter the path of valid file(s) to be compressed. Seperate multiple files by a comma (,): ";
    std::getline(std::cin, input);

    art.files = Utils::StringSplit(input, ','); 

    SetArtifact(art);
}

void NullArg::SetFlag(){
    flag = '\0';
}