#ifndef IMAGECOMPRESSIONENGINE_H
#define IMAGECOMPRESSIONENGINE_H

#include "./Batch.h"
#include <memory>
#include <vector>
#include <string>

class ImageCompressionEngine{
    public:
        ImageCompressionEngine(Batch& batch);

        int StartBatchCompression();
    private:
        bool GetNextBatchItem(unsigned short int batchItemId);
        void EncodeImage(File& currentFile);
        void DecodeImage(File& currentFile);
        void CreateLocalCopies(File& currentFile);

        Batch batch;        
};

#endif