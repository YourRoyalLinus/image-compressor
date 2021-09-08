#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "Context.h"
#include "HuffmanEncodingContext.h"
#include "FileMarshaller.h"

class Compressor{
    public:
        static void CreateContext(Context& context);
        static void EncodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller);
        static void DecodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller);
};

#endif