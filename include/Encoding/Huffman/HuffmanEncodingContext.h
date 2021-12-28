#ifndef HUFFMANENCODINGCONTEXT_H
#define HUFFMANENCODINGCONTEXT_H

#include "../EncodingContext.h"
#include "../../Artifacts//Huffman/HuffmanTreeNode.h"
#include "../../../include/BinaryHandling/BinaryWriter.h"
#include <vector>
#include <string>

class HuffmanEncodingContext : public EncodingContext{
    public:
        HuffmanEncodingContext(const char* filePath);
        HuffmanEncodingContext(File& f);
        void Build() override;
        void Encode(File& currentFile, FileMarshaller& marshaller) override;
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::shared_ptr<HuffmanTreeNode> rootNode;
        std::vector<std::string> encodedPixelVec;
        int encodedPixelDataBits;

        void BuildHuffmanContext();

        std::shared_ptr<std::ofstream> GetEncodedFileStream(File& currentFile, FileMarshaller& marshaller);
        int SeralizeAndWriteTo(std::ofstream& encodedFileStream, unsigned int fileOffset);
        void WriteHeaderDataTo(BinaryWriter& binWriter, BMPImage& img);
        void WriteEncodedDataTo(BinaryWriter& binWriter);
    
};

#endif