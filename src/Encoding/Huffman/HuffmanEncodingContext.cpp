#include "../../../include/Encoding/ContextBuildHelper.h"
#include "../../../include/Encoding/HuffmanContextBuildHelper.h"
#include "../../../include/Encoding/Huffman/HuffmanEncodingContext.h"
#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include <cstdio>
#include <fstream>
#include <algorithm>

HuffmanEncodingContext::HuffmanEncodingContext(const char* filePath){
    decodingStrategy = std::unique_ptr<HuffmanDecodingStrategy>(new HuffmanDecodingStrategy());
    contextFilePath = filePath;
}

HuffmanEncodingContext::HuffmanEncodingContext(File& f){
    decodingStrategy = std::unique_ptr<HuffmanDecodingStrategy>(new HuffmanDecodingStrategy());
    contextFilePath = f.fullPath.c_str();
}

void HuffmanEncodingContext::Build(){
    BuildImage();
    BuildHuffmanContext();
}

void HuffmanEncodingContext::BuildHuffmanContext(){
    std::shared_ptr<BMPImage> img = EncodingContext::GetBMPImage();

    int pixelBufferSize = img->cimage.size();
    int imageWidth = img->header->imageWidth;
    int imageHeight = img->header->imageHeight;
    unsigned char* pixelDataArray = img->pixelDataArray;

    HuffmanTree* huffmanTree = 0;
    PixelFrequencies* pixFreqs = 0; 

    std::vector<int> hist = ContextBuilder::GetHistogram(pixelBufferSize, pixelDataArray);
    int nonZeroNodes = ContextBuilder::GetNonZeroOccurances(hist);
    float probability = ContextBuilder::GetMinimumProbabilityOfOccurance(hist, imageWidth, imageHeight);
    int totalNodes = ContextBuilder::GetTotalNodes(nonZeroNodes);
    int maxCodeLength = ContextBuilder::GetMaxCodeLength(probability);

    pixFreqs = ContextBuilder::InitializePixelFrequencies(totalNodes, maxCodeLength);
    huffmanTree = ContextBuilder::InitializeHuffmanTree(nonZeroNodes);

    ContextBuilder::InitializeLeafNodes(hist, (imageWidth * imageHeight), pixFreqs, huffmanTree);
    ContextBuilder::SortHuffCodeArray(nonZeroNodes, huffmanTree);
    ContextBuilder::CreateHuffmanTree(nonZeroNodes, pixFreqs, huffmanTree);
    ContextBuilder::Backtrack(nonZeroNodes, totalNodes, pixFreqs);

    std::unordered_map<int, std::string> encodingMap = ContextBuilder::CreateEncodingMap(nonZeroNodes, pixFreqs);
    encodedPixelVec = ContextBuilder::CreateEncodedPixelVec(encodingMap, pixelBufferSize, pixelDataArray);
    encodedPixelDataBits = ContextBuilder::GetEncodedPixelDataSizeInBits(encodedPixelVec);
    
    std::shared_ptr<HuffmanTreeNode> huffmanRootNode = ContextBuilder::CreateHuffmanTreeNodes(pixFreqs, huffmanTree, totalNodes);

    rootNode = huffmanRootNode;
}

void HuffmanEncodingContext::Encode(File& currentFile, FileMarshaller& marshaller){
    int pixVal;
    int pixelDataSize = 0;
    int pixelDataArraySize = 0;

    std::shared_ptr<BMPImage> img = EncodingContext::GetBMPImage();

    int imageSize = img->cimage.size();
    unsigned char* pixelDataArray = img->pixelDataArray;
    int imageHeaderSize = img->header->size;

    {
        std::ofstream encodedImageStream = GetEncodedFileStream(currentFile, marshaller); 

        int dhtSerializedSize = SeralizeAndWriteTo(encodedImageStream, imageHeaderSize);
        int pixelDataOffset = imageHeaderSize + dhtSerializedSize;

        int bitPadding = WriteEncodedDataTo(encodedImageStream, pixelDataOffset);
        encodedPixelDataBits += bitPadding;

        int encodedFileSize = imageHeaderSize + dhtSerializedSize + (encodedPixelDataBits/8);

        img->header->reservedByteOne = currentFile.type;
        img->header->reservedByteTwo = static_cast<unsigned short>(bitPadding);
        img->header->compression = 3;
        img->header->pixelDataOffset = pixelDataOffset;
        img->header->imageSize = encodedFileSize;

        unsigned char* headerBuffer = img->header->WriteToBuffer();
        WriteHeaderDataTo(encodedImageStream, headerBuffer, imageHeaderSize);
        encodedImageStream.flush();
    }
}

void HuffmanEncodingContext::Decode(File& currentFile, FileMarshaller& marshaller){
    decodingStrategy->Decode(currentFile, marshaller);
}

std::ofstream HuffmanEncodingContext::GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller){
    marshaller.FlagFileForCleanUp(currentFile.fullPath);
    marshaller.UpdateFileExt(currentFile, "jcif");
    return marshaller.CreateOutfileStream(currentFile.fullPath, std::ofstream::binary);
}

int HuffmanEncodingContext::SeralizeAndWriteTo(std::ofstream& encodedFileStream, unsigned int fileOffset){
    encodedFileStream.seekp(fileOffset);
    {
        cereal::BinaryOutputArchive binaryOutputArchive(encodedFileStream);
        binaryOutputArchive(rootNode);
    }
    int end = encodedFileStream.tellp();
    
    return end - fileOffset;
}

void HuffmanEncodingContext::WriteHeaderDataTo(std::ofstream& encodedFileStream, unsigned char* headerBuf, int headerBufSize){
    encodedFileStream.seekp(0);
    encodedFileStream.write((char*)headerBuf, headerBufSize);
}

int HuffmanEncodingContext::WriteEncodedDataTo(std::ofstream& encodedFileStream, int fileOffset){
    encodedFileStream.seekp(fileOffset);
    int end = encodedPixelVec.size() - 1;
    int bits = 0;
    unsigned char byte = 0;
    unsigned char bit;

    //BIT MANIPULATOR CLASS
    for(unsigned int i = 0; i < encodedPixelVec.size(); i++){
        for(unsigned j = 0; j < encodedPixelVec[i].size(); j++){
            byte <<= 1;
            bit = encodedPixelVec[i][j];

            if((bit ^ '1') == 0){ 
                byte |= 1;
            }

            bits++;
            

            if(bits %  8 == 0){
                WriteByteToFile(encodedFileStream, &byte);
                byte = 0;
            }         
        }
    }

    unsigned short int padding = 0;
    if(bits % 8 != 0){
        while(bits % 8 != 0){
            bits++;
            byte <<= 1;
            padding++;
        }

        WriteByteToFile(encodedFileStream, &byte);
    }
    
    return padding;
}

void HuffmanEncodingContext::WriteByteToFile(std::ofstream& encodedFileStream, unsigned char* byteArray){ //RENAMING
    encodedFileStream.write((char*) byteArray, sizeof(char));
}

