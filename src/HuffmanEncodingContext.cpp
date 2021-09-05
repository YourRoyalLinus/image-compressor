#include "../include/HuffmanEncodingContext.h"
#include <cstdio>
#include <fstream>

HuffmanEncodingContext::HuffmanEncodingContext(const char* filePath){
    contextFilePath = filePath;
}

HuffmanEncodingContext::HuffmanEncodingContext(File& f){ //NEW
    contextFilePath = f.fullPath.c_str();
}

HuffmanEncodingContext::~HuffmanEncodingContext(){
}

HuffmanTree* HuffmanEncodingContext::GetHuffmanTree(){
    Artifact* ptr = Context::GetArtifact(Artifact::ArtifactType::HUFFMANTREE);
    HuffmanTree* ht = dynamic_cast<HuffmanTree*>(ptr);
    return ht;
}


HuffmanTable* HuffmanEncodingContext::GetHuffmanTable(){
    Artifact* ptr = Context::GetArtifact(Artifact::ArtifactType::HUFFMANTABLE);
    HuffmanTable* ht = dynamic_cast<HuffmanTable*>(ptr);
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
    HuffmanTable* huffmanTable = 0; 
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

    huffmanTable = ContextBuildHelper::CreateHuffmanTable(nonZeroNodes, pixFreqs);

    ContextBuildHelper::PopulateCodeLengths(nonZeroNodes, pixelBufferSize, pixelDataArray, pixFreqs, huffmanTable);

    Context::AddArtifact(Artifact::ArtifactType::HUFFMANTREE, huffmanTree);
    Context::AddArtifact(Artifact::ArtifactType::HUFFMANTABLE, huffmanTable);
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
        
        int dhtSerializedSize = SeralizeAndWriteTo(encodedImageStream, *this, imageHeaderSize);
        int pixelDataOffset = imageHeaderSize + dhtSerializedSize;
        int encodedFileSize = imageHeaderSize + dhtSerializedSize + 0;//GETPIXELDATASIZE
        GetBMPImage()->header->compression = 3;
        GetBMPImage()->header->pixelDataOffset = pixelDataOffset;
        GetBMPImage()->header->imageSize = encodedFileSize;

        unsigned char* headerBuffer = GetBMPImage()->header->WriteToBuffer();
        
        WriteHeaderDataTo(encodedImageStream, headerBuffer, imageHeaderSize);
        encodedImageStream.flush();
    }
      
}

//MOVED OUT PROB
std::ofstream HuffmanEncodingContext::GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller){
    std::string encodedFilePath = currentFile.relativePath + "/" + currentFile.name + "_encoded.bin";
    //CHANGE FILE TO ENCODED FILE PATH
    return marshaller.CreateFileStream(encodedFilePath, std::ofstream::binary);
}

int HuffmanEncodingContext::SeralizeAndWriteTo(std::ofstream& encodedFileStream, HuffmanEncodingContext& context, unsigned int fileOffset){
    encodedFileStream.seekp(fileOffset);
    {
        cereal::BinaryOutputArchive binaryOutputArchive(encodedFileStream);
        binaryOutputArchive(*GetHuffmanTable());
    }
    int end = encodedFileStream.tellp();
    
    return end - fileOffset;
}

void HuffmanEncodingContext::WriteHeaderDataTo(std::ofstream& encodedFileStream, unsigned char* headerBuf, int headerBufSize){
    encodedFileStream.seekp(0);
    encodedFileStream.write((char*)headerBuf, headerBufSize);
}