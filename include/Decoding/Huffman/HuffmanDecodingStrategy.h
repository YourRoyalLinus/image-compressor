#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../ImageHandling/ImageObjects/JCIFImage.h"
#include "../../Artifacts/Huffman/HuffmanTreeNode.h"
#include "../../../include/BinaryHandling/BinaryReader.h"
#include "../../../lib/CImg-2.9.9/CImg.h"
#include <vector>

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::shared_ptr<std::ifstream> GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller);
        std::shared_ptr<HuffmanTreeNode> DeserializeFileData(std::ifstream& encodedFileStream, int fileOffset);
        std::vector<unsigned char> DecodePixelArray(BinaryReader& binReader, JCIFImage& encodedImage, std::shared_ptr<HuffmanTreeNode> rootNode);
        void CreateImageFile(std::vector<unsigned char>& decodedPixelVec, File& currentFile, JCIFImage& encodedImage);
};

#endif