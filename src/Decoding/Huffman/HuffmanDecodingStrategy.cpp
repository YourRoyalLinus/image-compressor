#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include "../../../include/FileHandling/FileConverter.h"
#include "../../../include/ImageHandling/ImageParser.h"
#include <fstream>
#include <ios>

void HuffmanDecodingStrategy::Decode(File& currentFile, FileMarshaller& marshaller){
    JCIFImage encodedImage(currentFile.fullPath.c_str());
    ImageParser::instance().ParseImage(encodedImage);

    {
        std::shared_ptr<std::ifstream> encodedImageStream = GetDecodedFileStream(currentFile, marshaller);
        std::shared_ptr<HuffmanTreeNode> deserializedRootNode = DeserializeFileData(*encodedImageStream, encodedImage.header->size);  

        BinaryReader binReader = BinaryReader(encodedImageStream, encodedImage.encodedPixelArrayBytes, encodedImage.bitPadding);

        std::vector<unsigned char> decodedPixelVec = DecodePixelArray(binReader, deserializedRootNode);
        CreateImageFile(decodedPixelVec, currentFile, encodedImage);
    }
    
    currentFile.size = marshaller.GetFileSize(currentFile.fullPath);
}

std::shared_ptr<std::ifstream> HuffmanDecodingStrategy::GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller){
    marshaller.FlagFileForCleanUp(currentFile.fullPath);
    return marshaller.CreateInfileStream(currentFile.fullPath, std::ifstream::binary);
}

std::shared_ptr<HuffmanTreeNode> HuffmanDecodingStrategy::DeserializeFileData(std::ifstream& encodedFileStream, int fileOffset){
    encodedFileStream.seekg(fileOffset);

    std::shared_ptr<HuffmanTreeNode> tmpRoot;
    {
        cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
        binaryInputArchive(tmpRoot);
    }
    return tmpRoot;
}


std::vector<unsigned char> HuffmanDecodingStrategy::DecodePixelArray(BinaryReader& binReader, std::shared_ptr<HuffmanTreeNode> rootNode){
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


void HuffmanDecodingStrategy::CreateImageFile(std::vector<unsigned char>& decodedPixelVec, File& currentFile, JCIFImage& encodedImage){
     cimg_library::CImg<unsigned char> newImage(decodedPixelVec.data(), encodedImage.header->imageWidth, encodedImage.header->imageHeight, 1, 3); 

    switch(encodedImage.originalFileType){
        case File::FileType::BMP:
            FileConverter::instance().ConvertImageToBMP(currentFile, newImage);
            break;
        case File::FileType::TIFF:
            FileConverter::instance().ConvertImageToTIFF(currentFile, newImage);
            break;
        default:
            std::cout << "Error Decoding image: " << encodedImage.path << std::endl;
            break;
    }
}