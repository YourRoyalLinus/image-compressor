#ifndef HUFFMANENCODINGCONTEXT_H
#define HUFFMANENCODINGCONTEXT_H

#include "ContextBuildHelper.h"
#include "EncodingContext.h"
#include "HuffmanTree.h"
#include "HuffmanTable.h"

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
    private:
        void BuildHuffmanContext();
        
        std::ofstream GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller);
        int SeralizeAndWriteTo(std::ofstream& encodedFileStream, HuffmanEncodingContext& context, unsigned int fileOffset);
        void WriteHeaderDataTo(std::ofstream& encodedFileStream, unsigned char* headerBuf, int headerBufSize);

    
};

#endif