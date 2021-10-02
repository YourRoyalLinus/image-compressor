#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
#include <fstream>
#include <vector>
#include <ios>
#include <bitset>

HuffmanDecodingStrategy::HuffmanDecodingStrategy(){
}

HuffmanDecodingStrategy::~HuffmanDecodingStrategy(){

}

void HuffmanDecodingStrategy::Decode(File& currentFile, FileMarshaller& marshaller){
    const unsigned int headerSize = 54; //WHO HOLDS THIS

    {
        std::ifstream decodedImageStream = GetDecodedFileStream(currentFile, marshaller); 
        FileHeader* headerData = GetHeaderData(decodedImageStream, headerSize);
        HuffmanEncoded deserializedHuffmanEncoded = DeserializeFileData(decodedImageStream);
        
        int encodedPixelArraySize = deserializedHuffmanEncoded.encodedPixelArraySize; //(headerData->imageSize - headerData->pixelDataOffset);
        unsigned char* encodedPixelArr = new unsigned char[encodedPixelArraySize];
        std::cout << "BYTES = " << encodedPixelArraySize << " BITS =" << encodedPixelArraySize*8 << std::endl;
        decodedImageStream.read((char*) encodedPixelArr, encodedPixelArraySize/8);
        
        unsigned char* decodedPixelArray = DecodeNextHuffmanCode(encodedPixelArr, deserializedHuffmanEncoded);
        
        TestDecoding(decodedPixelArray, headerData->imageWidth, headerData->imageHeight);
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

HuffmanEncoded HuffmanDecodingStrategy::DeserializeFileData(std::ifstream& encodedFileStream){
    HuffmanEncoded tmpHuffEncoded;
    {
        cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
        binaryInputArchive(tmpHuffEncoded);
    }
    return tmpHuffEncoded;
}

unsigned char* HuffmanDecodingStrategy::DecodeNextHuffmanCode(unsigned char* encodedPixelArray, HuffmanEncoded huffEncoded){
    
    std::shared_ptr<HuffmanTreeNode> currentNode = huffEncoded.rootNode;
    std::vector<unsigned char> decodedPixelArr;

    int byteIndex = 0;
    int bitIndex = 0;
    int bits = 0;
    std::string nextCodeByte = std::bitset<8>(encodedPixelArray[byteIndex++]).to_string();
    std::string code;
    //Bit Manipulator // Bit shifting optimization
    while(bits < huffEncoded.encodedPixelArraySize-7){
        if(bitIndex >= 8){
            nextCodeByte = std::bitset<8>(encodedPixelArray[byteIndex++]).to_string();
            bitIndex = 0;
        }

        if(currentNode->left == nullptr && currentNode->right == nullptr){
            if(currentNode->pix > 255){
                std::cout << "AT " << byteIndex << "CODE = " << code <<  " and PIX = " << currentNode->pix << std::endl;
            }
            decodedPixelArr.push_back(currentNode->pix);
            currentNode = huffEncoded.rootNode;
            code = "";
        }

        char nextCode = nextCodeByte[bitIndex++];
        if(nextCode == '0'){
            code += '0';
            currentNode = currentNode->left;
        }
        else{
            code += '1';
            currentNode = currentNode->right;
        }
        bits++;        
    }
    std::cout << bits << std::endl;

    return &decodedPixelArr[0];
}
void HuffmanDecodingStrategy::TestDecoding(unsigned char* pixelArr, int width, int height){
    cimg_library::CImg<unsigned char> newImage(pixelArr, width, height, 1, 3);
    newImage.save_bmp("/home/jon/ImageCompressor/test_decoding_refactored.bmp");
}   