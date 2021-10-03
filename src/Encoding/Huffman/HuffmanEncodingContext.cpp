#include "../../../include/Encoding/Huffman/HuffmanEncodingContext.h"
#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include <cstdio>
#include <fstream>
#include <algorithm>

HuffmanEncodingContext::HuffmanEncodingContext(const char* filePath){
    decodingStrategy = new HuffmanDecodingStrategy();
    contextFilePath = filePath;
}

HuffmanEncodingContext::HuffmanEncodingContext(File& f){ //NEW
    decodingStrategy = new HuffmanDecodingStrategy();
    contextFilePath = f.fullPath.c_str();
}

HuffmanEncodingContext::~HuffmanEncodingContext(){
}

HuffmanTree* HuffmanEncodingContext::GetHuffmanTree(){
    Artifact* ptr = Context::GetArtifact(Artifact::ArtifactType::HUFFMANTREE);
    HuffmanTree* ht = dynamic_cast<HuffmanTree*>(ptr);
    return ht;
}

void HuffmanEncodingContext::Build(){
    BuildImage();
    BuildHuffmanContext();
}

void HuffmanEncodingContext::BuildHuffmanContext(){
    BMPImage* img = EncodingContext::GetBMPImage();

    int pixelBufferSize = img->cimage.size();
    int imageWidth = img->header->imageWidth;
    int imageHeight = img->header->imageHeight;
    unsigned char* pixelDataArray = img->pixelDataArray;

    HuffmanTree* huffmanTree = 0;
    PixelFrequencies* pixFreqs = 0; 

    int* hist = ContextBuildHelper::GetHistogram(pixelBufferSize, pixelDataArray);
    int nonZeroNodes = ContextBuildHelper::GetNonZeroOccurances(hist);
    float probability = ContextBuildHelper::GetMinimumProbabilityOfOccurance(hist, imageWidth, imageHeight);
    int totalNodes = ContextBuildHelper::GetTotalNodes(nonZeroNodes);
    int maxCodeLength = ContextBuildHelper::GetMaxCodeLength(probability);

    pixFreqs = ContextBuildHelper::InitializePixelFrequencies(totalNodes, maxCodeLength);
    huffmanTree = ContextBuildHelper::InitializeHuffmanTree(nonZeroNodes);

    ContextBuildHelper::InitializeLeafNodes(hist, (imageWidth * imageHeight), pixFreqs, huffmanTree);
    ContextBuildHelper::SortHuffCodeArray(nonZeroNodes, huffmanTree);
    ContextBuildHelper::CreateHuffmanTree(nonZeroNodes, pixFreqs, huffmanTree);
    ContextBuildHelper::Backtrack(nonZeroNodes, totalNodes, pixFreqs);

    std::unordered_map<int, std::string> encodingMap = ContextBuildHelper::CreateEncodingMap(nonZeroNodes, pixFreqs);
    encodedPixelVec = ContextBuildHelper::CreateEncodedPixelVec(encodingMap, pixelBufferSize, pixelDataArray);
    encodedPixelDataBits = ContextBuildHelper::GetEncodedPixelDataSizeInBits(encodedPixelVec);
    
    std::shared_ptr<HuffmanTreeNode> huffmanRootNode = ContextBuildHelper::CreateHuffmanTreeNodes(pixFreqs, huffmanTree, totalNodes);

    rootNode = huffmanRootNode;

    Context::AddArtifact(Artifact::ArtifactType::HUFFMANTREE, huffmanTree);
    Context::AddArtifact(Artifact::ArtifactType::PIXELFREQUENCIES, pixFreqs);
}

void HuffmanEncodingContext::Encode(File& currentFile, FileMarshaller& marshaller){
    int pixVal;
    int pixelDataSize = 0;
    int pixelDataArraySize = 0;

    int imageSize = GetBMPImage()->cimage.size();
    unsigned char* pixelDataArray = GetBMPImage()->pixelDataArray;
    int imageHeaderSize = GetBMPImage()->header->size;

    {
        std::ofstream encodedImageStream = GetEncodedFileStream(currentFile, marshaller); 

        int dhtSerializedSize = SeralizeAndWriteTo(encodedImageStream, imageHeaderSize);
        int pixelDataOffset = imageHeaderSize + dhtSerializedSize;

        int bitPadding = WriteEncodedDataTo(encodedImageStream, pixelDataOffset);
        encodedPixelDataBits += bitPadding;

        int encodedFileSize = imageHeaderSize + dhtSerializedSize + (encodedPixelDataBits/8);

        GetBMPImage()->header->compression = 3;
        GetBMPImage()->header->pixelDataOffset = pixelDataOffset;
        GetBMPImage()->header->imageSize = encodedFileSize;

        unsigned char* headerBuffer = GetBMPImage()->header->WriteToBuffer();
        WriteHeaderDataTo(encodedImageStream, headerBuffer, imageHeaderSize);
        encodedImageStream.flush();
    }
}

void HuffmanEncodingContext::Decode(File& currentFile, FileMarshaller& marshaller){
    decodingStrategy->Decode(currentFile, marshaller);
}


//MOVED OUT PROB
std::ofstream HuffmanEncodingContext::GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller){
    std::string encodedFilePath = currentFile.relativePath + "/" + currentFile.name + "_encoded.bin";
    //CHANGE FILE TO ENCODED FILE PATH
    return marshaller.CreateOutfileStream(encodedFilePath, std::ofstream::binary);
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

