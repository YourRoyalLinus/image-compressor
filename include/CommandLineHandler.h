#ifndef COMMANDLINEHANDLER_H
#define COMMANDLINEHANDLER_H

#include "../include/CommandLineArg.h"
#include <getopt.h>

class CommandLineHandler{
    public:
        ~CommandLineHandler();
        void ParseArgs(int argc, char **argv);
        void ProcessArg();
        CommandLineArg::Artifact RetrieveArtifact();

    private:
        void CreateArg(char userArg, char* optArg);

        inline static struct option longOptions[] = {
            {"help",    no_argument     , 0,  'h'},
            {"batch",   required_argument, 0, 'b'},
            {"file",    required_argument, 0, 'f'},
            {0,         0,                 0,  0 }
        };

        CommandLineArg* arg;

};

#endif