#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include <fstream>
#include <ios>

HuffmanDecodingStrategy::HuffmanDecodingStrategy(){
}

HuffmanDecodingStrategy::~HuffmanDecodingStrategy(){

}

void HuffmanDecodingStrategy::Decode(File& currentFile, FileMarshaller& marshaller){
    const unsigned int headerSize = 54; //WHO HOLDS THIS

    {
        std::ifstream decodedImageStream = GetDecodedFileStream(currentFile, marshaller); 
        FileHeader* headerData = GetHeaderData(decodedImageStream, headerSize);
        File::FileType originalFileType = (File::FileType) headerData->reservedByteOne;

        std::shared_ptr<HuffmanTreeNode> deserializedRootNode = DeserializeFileData(decodedImageStream);
        
        int encodedPixelArrayBytes = (headerData->imageSize - headerData->pixelDataOffset); 
        unsigned char* encodedPixelArr = new unsigned char[encodedPixelArrayBytes];
        decodedImageStream.read((char*) encodedPixelArr, encodedPixelArrayBytes);

        std::vector<unsigned char> decodedPixelVec = DecodePixelArray(encodedPixelArr, encodedPixelArrayBytes, deserializedRootNode);
        cimg_library::CImg<unsigned char> rawImg = CreateDecodedImage(decodedPixelVec, *headerData, currentFile);
        CreateImageFile(rawImg, currentFile, marshaller, originalFileType);
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

std::vector<unsigned char> HuffmanDecodingStrategy::DecodePixelArray(unsigned char* encodedPixelArray, int encodedPixelArrayBytes, std::shared_ptr<HuffmanTreeNode> rootNode){
    std::shared_ptr<HuffmanTreeNode> currentNode = rootNode;
    std::vector<unsigned char> decodedPixelVec;

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
            decodedPixelVec.push_back(currentNode->pix);
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

    return decodedPixelVec;
}

cimg_library::CImg<unsigned char> HuffmanDecodingStrategy::CreateDecodedImage(std::vector<unsigned char>& decodedPixelVec, FileHeader& headerData, File& currentFile){
    unsigned char* pixelArr = &decodedPixelVec[0];
    cimg_library::CImg<unsigned char> newImage(pixelArr, headerData.imageWidth, headerData.imageHeight, 1, 3); 
    return newImage;
}   

void HuffmanDecodingStrategy::CreateImageFile(cimg_library::CImg<unsigned char> img, File& currentFile, FileMarshaller& marshaller, File::FileType originalFileType){ //File cleanup on marshaller
    switch(originalFileType){
        case File::FileType::BMP:
            currentFile.fullPath =  currentFile.relativePath + "/" + currentFile.name +".bmp";
            currentFile.type = File::FileType::BMP;
            currentFile.ext = "bmp";
             
            img.save_bmp(currentFile.fullPath.c_str());
            break;
        case File::FileType::TIFF:
            currentFile.fullPath =  currentFile.relativePath + "/" + currentFile.name +".tiff";
            currentFile.type = File::FileType::TIFF;
            currentFile.ext = "tiff";

            img.save_tiff(currentFile.fullPath.c_str());
            break;
    }
}