#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../Encoding/Context.h"
#include "../Encoding/Huffman/HuffmanEncodingContext.h"
#include "../FileHandling/FileMarshaller.h"
#include "../FileHandling/FileObjects/File.h"
#include <memory>

namespace Compressor{
        std::unique_ptr<HuffmanEncodingContext> GetHuffmanEncodingContext(File& currentFile){
            return std::unique_ptr<HuffmanEncodingContext>(new HuffmanEncodingContext(currentFile.fullPath.c_str()));
        }

        void CreateContext(Context& context){
            context.Build();   
        }

        void EncodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller){
            encodingContext.Encode(currentFile, marshaller);
        }

        void DecodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller){
             encodingContext.Decode(currentFile, marshaller);
        }
}

#endif