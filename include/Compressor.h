#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../include/Context.h"
#include "../include/HuffmanEncodingContext.h"
#include <fstream>

class Compressor{
    public:
        static void CreateContext(Context& context);
        static const char * CreateEncodedFilePath(const char* partialPath);
        static void EncodeImageFile(const char* encodedFilePath, HuffmanEncodingContext& encodingContext);
    private:
        static std::ofstream CreateEncodedFile(const char* encodedFilePath);
        static int SerializeData(int fileOffset, const char* encodedFilePath, HuffmanEncodingContext& encodingContext);
};

#endif