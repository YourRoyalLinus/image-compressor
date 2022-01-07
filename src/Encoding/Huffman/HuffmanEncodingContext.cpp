#include "../../../include/FileHandling/FileMarshaller.h"
#include "../../../include/Encoding/Encoder.h"
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

    std::vector<int> hist = ContextBuilder::GetHistogram(pixelBufferSize, pixelDataArray);
    int nonZeroNodes = ContextBuilder::GetNonZeroOccurances(hist);
    int totalNodes = ContextBuilder::GetTotalNodes(nonZeroNodes);

    this->pixelFreqs = ContextBuilder::InitializePixelFrequencies();

    ContextBuilder::InitializeLeafNodes(hist, (imageWidth * imageHeight), this->pixelFreqs);
    ContextBuilder::SortPixFreqsAscending(this->pixelFreqs);  
    ContextBuilder::CreateHuffmanTree(nonZeroNodes, this->pixelFreqs);
    ContextBuilder::Backtrack(nonZeroNodes, totalNodes, this->pixelFreqs);

    ContextBuilder::SortPixelFreqsDescending(this->pixelFreqs);
    this->rootNode = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*this->pixelFreqs[0]));
}

void HuffmanEncodingContext::Encode(File& currentFile){
    std::shared_ptr<BMPImage> img = EncodingContext::GetBMPImage();
    int imageHeaderSize = img->header->size;
    int encodedPixelDataBits = 0;

    {
        std::shared_ptr<std::ofstream> encodedImageStream = Encoder::GetEncodedFileStream(currentFile); 

        int dhtSerializedSize = Encoder::SeralizeAndWriteTo(*encodedImageStream, this->rootNode, imageHeaderSize);
        int pixelDataOffset = imageHeaderSize + dhtSerializedSize;

        std::unique_ptr<BinaryWriter> binWriter = std::unique_ptr<BinaryWriter>(new BinaryWriter(encodedImageStream, pixelDataOffset));
        Encoder::WriteEncodedDataTo(*binWriter, *img, this->pixelFreqs, encodedPixelDataBits);

        encodedPixelDataBits += binWriter->GetPadding();
        int encodedFileSize = imageHeaderSize + dhtSerializedSize + (encodedPixelDataBits/8);

        img->header->reservedByteOne = currentFile.type;
        img->header->reservedByteTwo = binWriter->GetPadding();
        img->header->compression = 3;
        img->header->pixelDataOffset = pixelDataOffset;
        img->header->imageSize = encodedFileSize;
        
        binWriter.reset(new BinaryWriter(encodedImageStream, 0));
        Encoder::WriteHeaderDataTo(*binWriter, *img);
    }

    currentFile.size = FileMarshaller::instance().GetFileSize(currentFile.fullPath);
}

void HuffmanEncodingContext::Decode(File& currentFile){
    decodingStrategy->Decode(currentFile);
}