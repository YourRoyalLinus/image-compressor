#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "DecodingStrategy.h"
#include "HuffmanTable.h"

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        HuffmanDecodingStrategy();
        ~HuffmanDecodingStrategy();
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::ifstream GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller);
        FileHeader* GetHeaderData(std::ifstream& decodedFileStream, unsigned int headerSize); //WHICH CLASS HAS THIS RESPONSIBILITY? 
        HuffmanTable DeserializeFileData(std::ifstream& encodedFileStream, unsigned int fileOffset);
        void TestDecoding(unsigned char* pixelArr, int width, int height);
};

#endif