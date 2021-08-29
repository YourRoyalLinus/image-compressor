#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "../include/Compressor.h"
#include <bitset>


void Compressor::CreateContext(Context& context){
    context.Build();
    
}

void Compressor::EncodeImageFile(const char* encodedFilePath, HuffmanEncodingContext& encodingContext){
    int pixVal;
    int pixelDataSize = 0;
    int pixelDataArraySize = 0;

    int imageSize = encodingContext.GetBMPImage()->cimage.size();
    unsigned char* pixelDataArray = encodingContext.GetBMPImage()->pixelDataArray;
    int imageHeaderSize = 54;

    std::ofstream encodedImage = CreateEncodedFile(encodedFilePath);
    
    int dhtSerializedSize = SerializeData(imageHeaderSize, encodedFilePath, encodingContext);
    int pixelDataOffset = imageHeaderSize + dhtSerializedSize;
    int encodedFileSize = imageHeaderSize + dhtSerializedSize + 0;//GETPIXELDATASIZE
    
    encodingContext.GetBMPImage()->header->compression = 3;
    encodingContext.GetBMPImage()->header->pixelDataOffset = pixelDataOffset;
    encodingContext.GetBMPImage()->header->imageSize = encodedFileSize;
    std::cout << pixelDataOffset << std::endl;
    std::cout << encodedFileSize << std::endl;

    const unsigned char* headerBuffer = encodingContext.GetBMPImage()->header->WriteToBuffer();

    encodedImage.write((char*)headerBuffer, encodingContext.GetBMPImage()->header->size);
      
    encodedImage.close();   
}

std::ofstream Compressor::CreateEncodedFile(const char* filePath){
    std::ofstream encodedImage(filePath);
    if (!encodedImage.is_open())
    {
        std::cout << "Error opening file" << filePath << std::endl;
    }

    return encodedImage;
}

int Compressor::SerializeData(int fileOffset, const char* encodedFilePath, HuffmanEncodingContext& encodingContext){
    std::ofstream serializedOutputFile = std::ofstream(encodedFilePath, std::ofstream::binary);
    serializedOutputFile.seekp(fileOffset);
    HuffmanTable tmpHuffTable = *encodingContext.GetHuffmanTable();
    {
        cereal::BinaryOutputArchive binaryOutputArchive(serializedOutputFile);
        binaryOutputArchive(tmpHuffTable);
    }
    int end = serializedOutputFile.tellp();
    std::cout << "SerializedSize = " << end-fileOffset << std::endl;
    return end - fileOffset;
}

