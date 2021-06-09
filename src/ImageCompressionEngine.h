#ifndef IMAGECOMPRESSIONENGINE_H
#define IMAGECOMPRESSIONENGINE_H

#include "FileConverter.h"
#include "ImageCompressor.h"
#include <cstdio>
#include <vector>
#include <chrono>

class ImageCompressionEngine{
    public:
        ImageCompressionEngine();
        ImageCompressionEngine(std::string iDir, std::string oDir, std::string tsDir, std::vector<std::string> ifiles);
        ~ImageCompressionEngine();

        std::string CreateLocalInboundCopy();
        std::string CreateLocalOutboundCopy();
        int StartBatchCompression();

        void DisplayResults();
    private:
        void CleanupFiles(FileConverter::FileInfo* fileInfo);
        std::string inboundDir;
        std::string outboundDir;
        std::string timestampDir;

        std::vector<std::string> inputFiles;
        
        FILE* currentFile;       
        struct FileConverter::FileInfo* fileInfo;
        FileConverter* fileConverter;
        ImageCompressor* imageCompressor;


        std::chrono::time_point<std::chrono::system_clock> executeStart; //Batch class?
        std::chrono::time_point<std::chrono::system_clock> executeEnd;

};

#endif