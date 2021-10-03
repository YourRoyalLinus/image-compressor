#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../Artifacts/Huffman/HuffmanTreeNode.h"
#include <vector>

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        HuffmanDecodingStrategy();
        ~HuffmanDecodingStrategy();
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::ifstream GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller);
        FileHeader* GetHeaderData(std::ifstream& decodedFileStream, unsigned int headerSize); //WHICH CLASS HAS THIS RESPONSIBILITY? 
        std::shared_ptr<HuffmanTreeNode> DeserializeFileData(std::ifstream& encodedFileStream);
        unsigned char* DecodeNextHuffmanCode(unsigned char* encodedPixelArray, int encodedPixelArrayBytes, std::shared_ptr<HuffmanTreeNode> rootNode);
        void TestDecoding(unsigned char* pixelArr, int width, int height);
};

#endif