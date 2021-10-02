#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../Artifacts/Huffman/HuffmanEncoded.h"
#include <vector>

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        HuffmanDecodingStrategy();
        ~HuffmanDecodingStrategy();
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::ifstream GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller);
        FileHeader* GetHeaderData(std::ifstream& decodedFileStream, unsigned int headerSize); //WHICH CLASS HAS THIS RESPONSIBILITY? 
        HuffmanEncoded DeserializeFileData(std::ifstream& encodedFileStream);
        unsigned char* DecodeNextHuffmanCode(unsigned char* encodedPixelArray, HuffmanEncoded huffEncoded);
        void TestDecoding(unsigned char* pixelArr, int width, int height);
};

#endif