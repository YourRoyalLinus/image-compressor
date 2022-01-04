#include "../../../include/Encoding/ContextBuildHelper.h" //As a base for CBHs should this be included in HuffmanCBH?
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

    std::vector<int> hist = ContextBuilder::GetHistogram(pixelBufferSize, pixelDataArray);
    this->nonZeroNodes = ContextBuilder::GetNonZeroOccurances(hist);
    float probability = ContextBuilder::GetMinimumProbabilityOfOccurance(hist, imageWidth, imageHeight);
    int totalNodes = ContextBuilder::GetTotalNodes(nonZeroNodes);
    int maxCodeLength = ContextBuilder::GetMaxCodeLength(probability);

    this->pfs = ContextBuilder::InitializePixelFrequencies(totalNodes, maxCodeLength);
    huffmanTree = ContextBuilder::InitializeHuffmanTree(nonZeroNodes);

    ContextBuilder::InitializeLeafNodes(hist, (imageWidth * imageHeight), this->pfs, huffmanTree);
    ContextBuilder::SortHuffCodeArray(nonZeroNodes, huffmanTree);
    ContextBuilder::CreateHuffmanTree(nonZeroNodes, this->pfs, huffmanTree);
    ContextBuilder::Backtrack(nonZeroNodes, totalNodes, this->pfs);
    
    std::shared_ptr<HuffmanTreeNode> huffmanRootNode = ContextBuilder::CreateHuffmanTreeNodes(this->pfs,totalNodes);

    rootNode = huffmanRootNode;

    delete[] huffmanTree;
}

void HuffmanEncodingContext::Encode(File& currentFile, FileMarshaller& marshaller){
    std::shared_ptr<BMPImage> img = EncodingContext::GetBMPImage();
    int imageHeaderSize = img->header->size;

    {
        std::shared_ptr<std::ofstream> encodedImageStream = GetEncodedFileStream(currentFile, marshaller); 

        int dhtSerializedSize = SeralizeAndWriteTo(*encodedImageStream, imageHeaderSize);
        int pixelDataOffset = imageHeaderSize + dhtSerializedSize;

        std::unique_ptr<BinaryWriter> binWriter = std::unique_ptr<BinaryWriter>(new BinaryWriter(encodedImageStream, pixelDataOffset));
        WriteEncodedDataTo(*binWriter, *img);

        this->encodedPixelDataBits += binWriter->GetPadding();
        int encodedFileSize = imageHeaderSize + dhtSerializedSize + (this->encodedPixelDataBits/8);

        img->header->reservedByteOne = currentFile.type;
        img->header->reservedByteTwo = binWriter->GetPadding();
        img->header->compression = 3;
        img->header->pixelDataOffset = pixelDataOffset;
        img->header->imageSize = encodedFileSize;

        
        binWriter.reset(new BinaryWriter(encodedImageStream, 0));
        WriteHeaderDataTo(*binWriter, *img);
    }

    currentFile.size = marshaller.GetFileSize(currentFile.fullPath);
}

void HuffmanEncodingContext::Decode(File& currentFile, FileMarshaller& marshaller){
    decodingStrategy->Decode(currentFile, marshaller);
}

std::shared_ptr<std::ofstream> HuffmanEncodingContext::GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller){
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

void HuffmanEncodingContext::WriteHeaderDataTo(BinaryWriter& binWriter, BMPImage& img){
    unsigned char* headerBuffer = img.header->WriteToBuffer();
    int imageHeaderSize = img.header->size;
    unsigned char byte;

    for(unsigned int i = 0; i < imageHeaderSize; i++){
        byte = headerBuffer[i];
        binWriter.HandleNextByte(byte);
    }
}

void HuffmanEncodingContext::WriteEncodedDataTo(BinaryWriter& binWriter, BMPImage& img){
    unsigned char currentBit;
    int currentPixel;
    std::string huffmanCode;
    std::unordered_map<int, std::string> cache;

    for(int i = 0; i < img.cimage.size(); i++){
        currentPixel = *(img.pixelDataArray+i);

        if(cache.find(currentPixel) == cache.end()){
            huffmanCode = ContextBuilder::GetHuffmanCode(currentPixel, this->nonZeroNodes, this->pfs);
            cache.insert({currentPixel, huffmanCode});
        }
        else{
            huffmanCode = cache[currentPixel];
        }

        for(int j = 0; j < huffmanCode.size(); j++){
            currentBit = huffmanCode[j];
            binWriter.HandleNextBit(currentBit);
            this->encodedPixelDataBits++;
        }
    }    

    if(binWriter.IsPaddingRequired()){
        binWriter.CalculatePadding();
    }
}