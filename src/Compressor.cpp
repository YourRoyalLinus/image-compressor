#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"

#include "../include/Compressor.h"
#include "../include/FileMarshaller.h"



void Compressor::CreateContext(Context& context){
    context.Build();
    
}

void Compressor::EncodeImageFile(EncodingContext& encodingContext, File& currentFile, FileMarshaller& marshaller){
    encodingContext.Encode(currentFile, marshaller);
}


