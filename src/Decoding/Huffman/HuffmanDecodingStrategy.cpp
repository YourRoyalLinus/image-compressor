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
        std::shared_ptr<HuffmanTreeNode> deserializedRootNode = DeserializeFileData(decodedImageStream);
        
        int encodedPixelArrayBytes = (headerData->imageSize - headerData->pixelDataOffset); 
        unsigned char* encodedPixelArr = new unsigned char[encodedPixelArrayBytes];
        decodedImageStream.read((char*) encodedPixelArr, encodedPixelArrayBytes);

        unsigned char* decodedPixelArray = DecodeNextHuffmanCode(encodedPixelArr, encodedPixelArrayBytes, deserializedRootNode);
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

std::shared_ptr<HuffmanTreeNode> HuffmanDecodingStrategy::DeserializeFileData(std::ifstream& encodedFileStream){
    std::shared_ptr<HuffmanTreeNode> tmpRoot;
    {
        cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
        binaryInputArchive(tmpRoot);
    }
    return tmpRoot;
}

unsigned char* HuffmanDecodingStrategy::DecodeNextHuffmanCode(unsigned char* encodedPixelArray, int encodedPixelArrayBytes,  std::shared_ptr<HuffmanTreeNode> rootNode){
    std::shared_ptr<HuffmanTreeNode> currentNode = rootNode;
    std::vector<unsigned char> decodedPixelArr;

    int encodedPixelArrayBits = (encodedPixelArrayBytes*8);
    int byteIndex = 0;
    int bitIndex = 0;
    int bits = 0;

    unsigned char currentByte = encodedPixelArray[byteIndex++];
    int nextCode;
    int msb;
    //Bit Manipulator
    while(bits < encodedPixelArrayBits){
        if(bitIndex >= 8){
            currentByte = encodedPixelArray[byteIndex++];
            bitIndex = 0;
        }

        if(currentNode->left == nullptr && currentNode->right == nullptr){
            decodedPixelArr.push_back(currentNode->pix);
            currentNode = rootNode;
        }

        msb = 7-bitIndex++;
        nextCode = (currentByte >> msb) & 1;
        if((nextCode ^ 0) == 0){
            currentNode = currentNode->left;
        }
        else{
            currentNode = currentNode->right;
        }
        bits++;        
    }

    return &decodedPixelArr[0];
}
void HuffmanDecodingStrategy::TestDecoding(unsigned char* pixelArr, int width, int height){
    cimg_library::CImg<unsigned char> newImage(pixelArr, width, height, 1, 3);
    newImage.save_bmp("/home/jon/ImageCompressor/test_decoding_refactored.bmp");
}   