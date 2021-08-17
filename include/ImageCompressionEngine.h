#ifndef IMAGECOMPRESSIONENGINE_H
#define IMAGECOMPRESSIONENGINE_H

#include "../include/ImageCompressor.h"
#include "../include/FileMarshaller.h"
#include "../include/Batch.h"

#include <vector>
#include <string>

class ImageCompressionEngine{
    public:
        ImageCompressionEngine(Batch* batch);
        ImageCompressionEngine(Batch* batch, FileMarshaller* fm);
        ~ImageCompressionEngine();

        int StartBatchCompression();
    private:
        FileMarshaller* fileMarshaller;
        ImageCompressor* imageCompressor;
        Batch* batch;

        std::vector<std::string> tmpFilePathsToBeCleanedUp;
};

#endif