
#ifndef BATCH_H
#define BATCH_H

#include <chrono>
#include <vector>
#include <string>

class Batch{
    public:
        Batch(std::vector<std::string> files, std::string iPath, std::string oPath);

        void SetActiveItem(std::string item);
        void ExecuteStart();
        void ExecuteEnd();
        void RecordExecutionResults(long startSize, long compressedSize);
        
        void ItemSuccessfullyProcessed(bool status);
        int CheckBatchStatus();
        

        std::vector<std::string> inputFiles;
        std::string inboundPath;
        std::string outboundPath;
        
    private:
        
        std::chrono::time_point<std::chrono::system_clock> executeStart;
        std::chrono::time_point<std::chrono::system_clock> executeEnd;

        std::string activeItem;
        unsigned int batchSize;
        unsigned int successfulExecutions;
        

};

#endif