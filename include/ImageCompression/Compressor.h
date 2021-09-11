#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../Encoding/Context.h"
#include "../Encoding/Huffman/HuffmanEncodingContext.h"
#include "../FileHandling/FileMarshaller.h"

class Compressor{
    public:
        static void CreateContext(Context& context);
        static void EncodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller);
        static void DecodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller);
};

#endif