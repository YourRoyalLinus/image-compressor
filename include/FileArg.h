#ifndef FILEARG_H
#define FILEARG_H

#include "../include/CommandLineArg.h"

class FileArg : public CommandLineArg{
    public:
        FileArg(std::string filePathParam);
        void Handle() override;
        void SetFlag() override;
    private:
        std::string filePath;
};

#endif