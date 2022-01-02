#ifndef BATCHITEM_H
#define BATCHITEM_H

#include "./Batch.h"
#include "../FileHandling/FileObjects/File.h"
#include <memory>
#include <chrono>

class BatchItem{
    private:
        BatchItem(std::shared_ptr<File> f);
        void ExecuteStart();
        void ExecuteEnd();
        
        std::shared_ptr<File> _file;
        std::string originalPath;

        std::chrono::time_point<std::chrono::system_clock> executeStartTime;
        std::chrono::time_point<std::chrono::system_clock> executeEndTime;

        int startSize = 0;
        int endSize = 0;
        bool isEncoded;

    friend class Batch;
};

#endif