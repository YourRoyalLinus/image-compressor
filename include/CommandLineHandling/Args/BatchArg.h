#ifndef BATCHARG_H
#define BATCHARG_H

#include "../../CommandLineHandling/Args/CommandLineArg.h"
#include <string>
#include <fstream>

class BatchArg : public CommandLineArg{
    public:
        BatchArg(std::string filePathParam);
        void Handle() override;
        void SetFlag() override;
    private:
        std::string batchFilePath;
        void ReadInBatchFile();
        void CollectFilePathsFromBatchFile(std::fstream& batchFile);

};

#endif