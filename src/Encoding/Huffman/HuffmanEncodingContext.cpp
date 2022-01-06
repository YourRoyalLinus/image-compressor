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


    std::vector<int> hist = ContextBuilder::GetHistogram(pixelBufferSize, pixelDataArray);
    int nonZeroNodes = ContextBuilder::GetNonZeroOccurances(hist);
    int totalNodes = ContextBuilder::GetTotalNodes(nonZeroNodes);

    this->pixelFreqs = ContextBuilder::InitializePixelFrequencies();
    //std::vector<HuffmanCode> huffmanCodes = ContextBuilder::InitializeHuffmanCodes(nonZeroNodes);

    ContextBuilder::InitializeLeafNodes(hist, (imageWidth * imageHeight), this->pixelFreqs);
    ContextBuilder::SortPixFreqsAscending(this->pixelFreqs);  
    ContextBuilder::CreateHuffmanTree(nonZeroNodes, this->pixelFreqs);
    ContextBuilder::Backtrack(nonZeroNodes, totalNodes, this->pixelFreqs);

    ContextBuilder::SortPixelFreqsDescending(this->pixelFreqs);
    rootNode = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*this->pixelFreqs[0]));

    std::ofstream test("tree.txt");
    test << "i          |             pix                   | code \n";
    for(int i = 0; i < pixelFreqs.size(); i++){
        test << i << "          " << pixelFreqs[i]->pix << "          " << pixelFreqs[i]->code << std::endl;
    }
    test.flush();
    test.close();
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

    for(int i = 0; i < imageHeaderSize; i++){
        byte = headerBuffer[i];
        binWriter.HandleNextByte(byte);
    }
}

void HuffmanEncodingContext::WriteEncodedDataTo(BinaryWriter& binWriter, BMPImage& img){
    unsigned char currentBit;
    int currentPixel;
    std::string huffmanCode;
    std::unordered_map<int, std::string> cache;

    for(unsigned long i = 0; i < img.cimage.size(); i++){
        currentPixel = *(img.pixelDataArray+i);

        if(cache.find(currentPixel) == cache.end()){
            huffmanCode = ContextBuilder::GetHuffmanCode(currentPixel, this->pixelFreqs);
            cache.insert({currentPixel, huffmanCode});
        }
        else{
            huffmanCode = cache[currentPixel];
        }
        
        for(unsigned long j = 0; j < huffmanCode.size(); j++){
            currentBit = huffmanCode[j];
            binWriter.HandleNextBit(currentBit);
            this->encodedPixelDataBits++;
        }
    }    

    if(binWriter.IsPaddingRequired()){
        binWriter.CalculatePadding();
    }
}