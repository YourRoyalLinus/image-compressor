#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../ImageHandling/ImageObjects/JCIFImage.h"
#include "../../Artifacts/Huffman/HuffmanTreeNode.h"
#include "../../../lib/CImg-2.9.9/CImg.h"
#include <vector>

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        void Decode(File& currentFile, FileMarshaller& marshaller) override;
    private:
        std::ifstream GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller);
        void AdjustStreamPointer(std::ifstream& encodedFileStream, int pointerStart);
        std::shared_ptr<HuffmanTreeNode> DeserializeFileData(std::ifstream& encodedFileStream);
        unsigned char* FetchEncodedPixelArray(JCIFImage& encodedImage, std::ifstream& encodedFileStream);
        void ReadBytesInto(std::ifstream& encodedFileStream, unsigned char* encodedPixelDataArray, int bytesToRead);
        std::vector<unsigned char> DecodePixelArray(unsigned char* encodedPixelArray, JCIFImage& encodedImage, std::shared_ptr<HuffmanTreeNode> rootNode);
        cimg_library::CImg<unsigned char> CreateDecodedImage(std::vector<unsigned char>& decodedPixelVec, JCIFImage& encodedImage, File& currentFile);
        void CreateImageFile(cimg_library::CImg<unsigned char> img, File& currentFile, FileMarshaller& marshaller, JCIFImage& encodedImage);
};

#endif