#ifndef DECODER_H
#define DECODER_H

#include "../ImageHandling/ImageObjects/JCIFImage.h"
#include "../Artifacts/Huffman/HuffmanTreeNode.h"
#include "../FileHandling/FileMarshaller.h"
#include "../ImageHandling/ImageMarshaller.h"
#include "../../lib/CImg-2.9.9/CImg.h"
#include <memory>
#include <fstream>
#include <ios>
#include <iostream>


namespace Decoder{
    std::shared_ptr<JCIFImage> CreateEncodedImage(File& file){
        std::shared_ptr<JCIFImage> encodedImage = std::shared_ptr<JCIFImage>(new JCIFImage(file.fullPath.c_str()));
        ImageMarshaller::instance().ParseImage(encodedImage);
        return encodedImage;
    }

    std::shared_ptr<std::ifstream> GetDecodedFileStream(File& currentFile){
        FileMarshaller::instance().FlagFileForCleanUp(currentFile.fullPath);
        return FileMarshaller::instance().CreateInfileStream(currentFile.fullPath, std::ifstream::binary);
    }

    std::shared_ptr<HuffmanTreeNode> DeserializeFileData(std::ifstream& encodedFileStream, int fileOffset){
         encodedFileStream.seekg(fileOffset);

        std::shared_ptr<HuffmanTreeNode> tmpRoot;
        {
            cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
            binaryInputArchive(tmpRoot);
        }
        return tmpRoot;
    }

    std::vector<unsigned char> DecodePixelArray(BinaryReader& binReader, std::shared_ptr<HuffmanTreeNode>& rootNode){
        std::shared_ptr<HuffmanTreeNode> currentNode = rootNode;
        std::vector<unsigned char> decodedPixelVec;
        int nextCode;
        
        while(!binReader.AllBitsRead()){   
            nextCode = binReader.GetNextCode();

            if(currentNode->left == nullptr && currentNode->right == nullptr){
                decodedPixelVec.push_back(currentNode->pix);
                currentNode = rootNode;
            }

            if((nextCode ^ 0) == 0){
                currentNode = currentNode->left;
            }
            else{
                currentNode = currentNode->right;
            }
        }

        return decodedPixelVec;
    }

    void CreateImageFile(std::vector<unsigned char>& decodedPixelVec, File& currentFile, JCIFImage& encodedImage){
        cimg_library::CImg<unsigned char> newImage(decodedPixelVec.data(), encodedImage.header->imageWidth, encodedImage.header->imageHeight, 1, 3); 

        ImageMarshaller::instance().SaveImage(currentFile, encodedImage, newImage);
    }
}

#endif