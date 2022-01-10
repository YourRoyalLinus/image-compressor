#ifndef ENCODER_H
#define ENCODER_H

#include "../BinaryHandling/BinaryWriter.h"
#include "../ImageHandling/ImageObjects/BMPImage.h"
#include "../FileHandling/FileMarshaller.h"
#include "../Artifacts/Huffman/HuffmanTreeNode.h"
#include "../../lib/Cereal-1.3.0/include/cereal/archives/binary.hpp"

#include <memory>
#include <algorithm>
#include <unordered_map>

namespace Encoder{
    namespace{
        std::string GetHuffmanCode(int pixel, std::vector<std::shared_ptr<PixelFrequencies>>& pixFreqs, std::unordered_map<unsigned short int, std::string>& cache){
            if(cache.find(pixel) != cache.end()){
                return cache[pixel];
            }   
            
            for(std::size_t k = 0; k < pixFreqs.size(); k++){
                if(pixel == pixFreqs[k]->pix && (pixFreqs[k]->left == nullptr && pixFreqs[k]->right == nullptr)){
                    cache.insert({pixel, pixFreqs[k]->code});
                    return pixFreqs[k]->code;
                }
            }
            return "";
        }
    }

    std::shared_ptr<std::ofstream> GetEncodedFileStream(File& currentFile){
        FileMarshaller::instance().FlagFileForCleanUp(currentFile.fullPath);
        FileMarshaller::instance().UpdateFileExt(currentFile, "jcif");
        return FileMarshaller::instance().CreateOutfileStream(currentFile.fullPath, std::ofstream::binary);
    }

    int SeralizeAndWriteTo(std::ofstream& encodedFileStream, std::shared_ptr<HuffmanTreeNode>& rootNode, unsigned int fileOffset){
        encodedFileStream.seekp(fileOffset);
        {
            cereal::BinaryOutputArchive binaryOutputArchive(encodedFileStream);
            binaryOutputArchive(rootNode);
        }
        int end = encodedFileStream.tellp();
        
        return end - fileOffset;
    }

    void WriteHeaderDataTo(BinaryWriter& binWriter, BMPImage& img){
        std::vector<unsigned char> headerBuffer = img.header->WriteToBuffer();
        int imageHeaderSize = img.header->size;
        unsigned char byte;

        for(int i = 0; i < imageHeaderSize; i++){
            byte = headerBuffer[i];
            binWriter.HandleNextByte(byte);
        }
    }

    void WriteEncodedDataTo(BinaryWriter& binWriter, BMPImage& img, std::vector<std::shared_ptr<PixelFrequencies>>& pixelFreqs, int& encodedPixelDataBits){
        unsigned char currentBit;
        unsigned short int currentPixel;
        std::string huffmanCode;
        std::unordered_map<unsigned short int, std::string> cache;

        for(unsigned long i = 0; i < img.cimage.size(); i++){
            currentPixel = *(img.pixelDataArray+i);
            huffmanCode = GetHuffmanCode(currentPixel, pixelFreqs, cache);
            
            for(unsigned long j = 0; j < huffmanCode.size(); j++){
                currentBit = huffmanCode[j];
                binWriter.HandleNextBit(currentBit);
                encodedPixelDataBits++;
            }
        }

        if(binWriter.IsPaddingRequired()){
            binWriter.CalculatePadding();
        }
    }
}

#endif