#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "../Encoding/Context.h"
#include "../Encoding/Huffman/HuffmanEncodingContext.h"
#include "../FileHandling/FileObjects/File.h"
#include "../FileHandling/FileMarshaller.h"
#include <memory>

namespace Compressor{
    std::unique_ptr<HuffmanEncodingContext> GetHuffmanEncodingContext(File& currentFile){
        return std::unique_ptr<HuffmanEncodingContext>(new HuffmanEncodingContext(currentFile.fullPath.c_str()));
    }

    void CreateContext(Context& context){
        context.Build();   
    }

    bool ValidateFile(File& currentFile){
        bool valid = FileMarshaller::instance().IsValidFile(currentFile);
        if(!valid){
            std::cout << "." << currentFile.ext << " compression is not supported" << std::endl;
            return false;
        }
        else{
            return true;
        }
    }

    void EncodeImageFile(EncodingContext& encodingContext, File& currentFile){
        encodingContext.Encode(currentFile);
    }

    void DecodeImageFile(EncodingContext& encodingContext, File& currentFile){
            encodingContext.Decode(currentFile);
    }
}

#endif