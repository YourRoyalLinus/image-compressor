#ifndef IMAGECOMPRESSIONENGINE_H
#define IMAGECOMPRESSIONENGINE_H

#include "./Batch.h"
#include "../FileHandling/FileMarshaller.h"
#include <memory>
#include <vector>
#include <string>

class ImageCompressionEngine{
    public:
        ImageCompressionEngine(std::shared_ptr<Batch> batch);
        ImageCompressionEngine(std::shared_ptr<Batch> batch, std::shared_ptr<FileMarshaller> fm);

        int StartBatchCompression();
    private:
        bool GetNextBatchItem(unsigned short int batchItemId);
        void EncodeImage(File& currentFile);
        void DecodeImage(File& currentFile);
        void CreateLocalCopies(File& currentFile);

        std::shared_ptr<FileMarshaller> fileMarshaller;
        std::shared_ptr<Batch> batch;
};

#endif