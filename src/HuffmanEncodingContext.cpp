#include "../include/HuffmanEncodingContext.h"

HuffmanEncodingContext::HuffmanEncodingContext(const char* filePath){
    contextFilePath = filePath;
}

HuffmanEncodingContext::~HuffmanEncodingContext(){
}

HuffmanTree* HuffmanEncodingContext::GetHuffmanTree(){
    Artifact* ptr = Context::GetArtifact(Context::BuildArtifact::HUFFMANTREE);
    HuffmanTree* ht = dynamic_cast<HuffmanTree*>(ptr);
    return ht;
}


HuffmanTable* HuffmanEncodingContext::GetHuffmanTable(){
    Artifact* ptr = Context::GetArtifact(Context::BuildArtifact::HUFFMANTABLE);
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

    int* hist = ContextBuilderHelper::GetHistogram(pixelBufferSize, pixelDataArray);
    int nonZeroNodes = ContextBuilderHelper::GetNonZeroOccurances(hist);
    float probability = ContextBuilderHelper::GetMinimumProbabilityOfOccurance(hist, imageWidth, imageHeight);
 
    int totalNodes = ContextBuilderHelper::GetTotalNodes(nonZeroNodes);
    int maxCodeLength = ContextBuilderHelper::GetMaxCodeLength(probability);

    pixFreqs = ContextBuilderHelper::InitializePixelFrequencies(totalNodes, maxCodeLength);
    huffmanTree = ContextBuilderHelper::InitializeHuffmanTree(nonZeroNodes);

    ContextBuilderHelper::InitializeLeafNodes(hist, (imageWidth * imageHeight), pixFreqs, huffmanTree);
    ContextBuilderHelper::SortHuffCodeArray(nonZeroNodes, huffmanTree);
    ContextBuilderHelper::CreateHuffmanTree(nonZeroNodes, pixFreqs, huffmanTree);
    ContextBuilderHelper::Backtrack(nonZeroNodes, totalNodes, pixFreqs);

    huffmanTable = ContextBuilderHelper::CreateHuffmanTable(nonZeroNodes, pixFreqs);

    ContextBuilderHelper::PopulateCodeLengths(nonZeroNodes, pixelBufferSize, pixelDataArray, pixFreqs, huffmanTable);

    Context::AddArtifact(Context::BuildArtifact::HUFFMANTREE, huffmanTree);
    Context::AddArtifact(Context::BuildArtifact::HUFFMANTABLE, huffmanTable);
    Context::AddArtifact(Context::BuildArtifact::PIXELFREQUENCIES, pixFreqs);
}