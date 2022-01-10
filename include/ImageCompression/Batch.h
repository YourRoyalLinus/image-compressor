
#ifndef BATCH_H
#define BATCH_H

#include "./BatchItem.h"
#include "../FileHandling/FileObjects/File.h"
#include <memory>
#include <chrono>
#include <vector>
#include <string>

class BatchItem;

class Batch{
    public:
        Batch(std::vector<std::string>& files, std::string iPath, std::string oPath);

        std::shared_ptr<File> GetActiveItem();
        void SetActiveItem(std::shared_ptr<File> file);
        void UpdateActiveItemSize();
        
        void ItemExecuteStart();
        void ItemExecuteEnd();

        void InvalidBatchItem();
        void ValidateBatchItem();
        int CheckBatchStatus();
        
        std::vector<std::string> inputFiles;
        std::string inboundPath;
        std::string outboundPath;
    private:
        void ItemSuccessfullyProcessed(bool status);
        void RecordExecutionResults();

        std::shared_ptr<BatchItem> activeItem;
        unsigned int batchSize;
        unsigned int successfulExecutions;
        

};

#endif