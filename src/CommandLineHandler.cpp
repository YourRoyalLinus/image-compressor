#include "../include/CommandLineHandler.h"
#include "../include/HelpArg.h"
#include "../include/BatchArg.h"
#include "../include/FileArg.h"
#include "../include/InvalidArg.h"
#include "../include/NullArg.h"

CommandLineHandler::~CommandLineHandler(){
    if(arg != nullptr){
        delete(arg);
        arg = nullptr;
    }
}

void CommandLineHandler::ParseArgs(int argc, char **argv){
    int opt;
    int optionIndex = 0;
    int thisOptionOptInd = optind ? optind : 1;
    opt = getopt_long_only(argc, argv, "", longOptions, &optionIndex);

    CreateArg(opt, optarg);
}

void CommandLineHandler::ProcessArg(){
    arg->Handle();
}

CommandLineArg::Artifact CommandLineHandler::RetrieveArtifact(){
    return arg->GetArtifact();
}

void CommandLineHandler::CreateArg(char userArg, char* optArg){
    switch(userArg){
        case -1:
            arg = new NullArg();
            break;
        case 'h':
            arg = new HelpArg();
            break;
        case 'b':
            arg = new BatchArg(optarg);
            break;
        case 'f':
            arg = new FileArg(optarg);
            break;
        default:
            arg = new InvalidArg(userArg);
            break;
    }
}