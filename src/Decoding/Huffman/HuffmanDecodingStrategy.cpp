#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
#include <fstream>
#include <vector>

HuffmanDecodingStrategy::HuffmanDecodingStrategy(){
}

HuffmanDecodingStrategy::~HuffmanDecodingStrategy(){

}

void HuffmanDecodingStrategy::Decode(File& currentFile, FileMarshaller& marshaller){
    const unsigned int headerSize = 54; //WHO HOLDS THIS
    {
        std::ifstream decodedImageStream = GetDecodedFileStream(currentFile, marshaller); 
        FileHeader* headerData = GetHeaderData(decodedImageStream, headerSize);
        HuffmanTable desHuffmanTable = DeserializeFileData(decodedImageStream, headerSize);

        unsigned char* pixArr = new unsigned char[desHuffmanTable .codes.size()];
        std::vector<unsigned char> pixelArray(desHuffmanTable .codes.size());
        for(unsigned i = 0; i < desHuffmanTable .codes.size(); i++){
            std::string encodedPixelCode = desHuffmanTable .codes[i];
            int decodedPixelValue = desHuffmanTable .table[encodedPixelCode];
            pixArr[i] = decodedPixelValue & 0xFF;
        }
        
        TestDecoding(pixArr, headerData->imageWidth, headerData->imageHeight);
    }
}

std::ifstream HuffmanDecodingStrategy::GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller){
    return marshaller.CreateInfileStream(currentFile.fullPath, std::ifstream::binary);
}

FileHeader* HuffmanDecodingStrategy::GetHeaderData(std::ifstream& decodedImageStream, unsigned int headerSize){
    unsigned char headerBuffer[headerSize];
    decodedImageStream.readsome((char*)headerBuffer, headerSize);

    FileHeader* headerInfo = new FileHeader(headerBuffer);

    return headerInfo;
}

HuffmanTable HuffmanDecodingStrategy::DeserializeFileData(std::ifstream& encodedFileStream, unsigned int fileOffset){
    HuffmanTable tmpHuffTable;
    encodedFileStream.seekg(fileOffset);
    {
        cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
        binaryInputArchive(tmpHuffTable);
    }
    return tmpHuffTable;
}

void HuffmanDecodingStrategy::TestDecoding(unsigned char* pixelArr, int width, int height){
    cimg_library::CImg<unsigned char> newImage(pixelArr, width, height, 1, 3);
    newImage.save_bmp("/home/jon/ImageCompressor/test_decoding_refactored.bmp");
}   