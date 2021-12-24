#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../Artifacts/Huffman/HuffmanTreeNode.h"
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
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
        std::vector<unsigned char> DecodePixelArray(unsigned char* encodedPixelArray, int encodedPixelArrayBytes, int compressedPaddingBits, std::shared_ptr<HuffmanTreeNode> rootNode);
        cimg_library::CImg<unsigned char> CreateDecodedImage(std::vector<unsigned char>& decodedPixelVec, FileHeader& headerData, File& currentFile);
        void CreateImageFile(cimg_library::CImg<unsigned char> img, File& currentFile, FileMarshaller& marshaller, File::FileType originalFileType);
};

#endif