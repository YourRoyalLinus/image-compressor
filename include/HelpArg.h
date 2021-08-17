#ifndef HELPARG_H
#define HELPARG_H

#include "../include/CommandLineArg.h"

class HelpArg : public CommandLineArg{
    public:
        HelpArg();
        void Handle() override;
        void SetFlag() override;
    private:
        inline static std::string helpMsg = "This program can compress BMP and TIFF files.\n"
                            "If leave the command line empty, the program will prompt you for a file.\n"
                            "You can input multiple files in the command line by seperating each file path with a comma (,).\n"
                            "Flags: \n"
                            "--batch=BATCHFILE \t\t Text file(.txt) of files to be compressed. Seperated by newlines.\n"
                            "--file=FILEPATH \t\t File path of the file to be compressed.\n"
                            "--help \t\t\t\t Print this message and exit.\n";
};

#endif