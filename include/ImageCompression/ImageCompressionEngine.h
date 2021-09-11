#ifndef IMAGECOMPRESSIONENGINE_H
#define IMAGECOMPRESSIONENGINE_H

#include "./Batch.h"
#include "../FileHandling/FileMarshaller.h"

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
        Batch* batch;

        std::vector<std::string> tmpFilePathsToBeCleanedUp;
};

#endif