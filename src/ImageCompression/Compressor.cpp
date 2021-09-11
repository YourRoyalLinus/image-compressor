#include "../../include/ImageCompression/Compressor.h"
#include "../../include/FileHandling/FileMarshaller.h"



void Compressor::CreateContext(Context& context){
    context.Build();
    
}

void Compressor::EncodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller){
    encodingContext.Encode(currentFile, marshaller);
}

void Compressor::DecodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller){
    encodingContext.Decode(currentFile, marshaller);
}

