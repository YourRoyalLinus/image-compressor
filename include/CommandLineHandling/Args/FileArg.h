#ifndef FILEARG_H
#define FILEARG_H

#include "../../CommandLineHandling/Args/CommandLineArg.h"

class FileArg : public CommandLineArg{
    public:
        FileArg(std::string filePathParam);
        void Handle() override;
        void SetFlag() override;
    private:
        std::string filePath;
};

#endif