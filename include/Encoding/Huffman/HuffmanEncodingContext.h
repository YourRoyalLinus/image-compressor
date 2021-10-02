#ifndef HUFFMANENCODINGCONTEXT_H
#define HUFFMANENCODINGCONTEXT_H

#include "../ContextBuildHelper.h"
#include "../EncodingContext.h"
#include "../../Artifacts//Huffman/HuffmanTree.h"
#include "../../Artifacts/Huffman/HuffmanTable.h"
#include "../../Artifacts/Huffman/HuffmanEncoded.h"
#include <vector>
#include <string>

class HuffmanEncodingContext : public EncodingContext{
    public:
        HuffmanEncodingContext(const char* filePath);
        HuffmanEncodingContext(File& f); //NEW
        ~HuffmanEncodingContext();
        void Build() override;
        void Encode(File& currentFile, FileMarshaller& marshaller) override;
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
        HuffmanTree* GetHuffmanTree();
        HuffmanTable* GetHuffmanTable();
        HuffmanEncoded* GetHuffmanEncoded();

    private:
        std::vector<std::string> encodedPixelVec;

        void BuildHuffmanContext();
        std::ofstream GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller);
        int SeralizeAndWriteTo(std::ofstream& encodedFileStream, HuffmanEncodingContext& context, unsigned int fileOffset);
        void WriteHeaderDataTo(std::ofstream& encodedFileStream, unsigned char* headerBuf, int headerBufSize);
        int WriteEncodedDataTo(std::ofstream& encodedFileStream, int fileOffset, std::vector<std::string> encodedPixelVec);
        void WriteByteToFile(std::ofstream& encodedFileStream, unsigned char* byteArray);
    
};

#endif