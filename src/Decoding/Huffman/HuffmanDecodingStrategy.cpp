#include "../../../include/Decoding/Huffman/HuffmanDecodingStrategy.h"
#include "../../../include/ImageHandling/ImageParser.h"
#include <fstream>
#include <ios>

void HuffmanDecodingStrategy::Decode(File& currentFile, FileMarshaller& marshaller){
    std::unique_ptr<JCIFImage> encodedImage = std::unique_ptr<JCIFImage>(new JCIFImage(currentFile.fullPath.c_str()));
    ImageParser::instance().ParseImage(*encodedImage);
    {
        std::ifstream encodedImageStream = GetDecodedFileStream(currentFile, marshaller);
        AdjustStreamPointer(encodedImageStream, encodedImage->header->size);

        std::shared_ptr<HuffmanTreeNode> deserializedRootNode = DeserializeFileData(encodedImageStream);
        
        unsigned char* encodedPixelArray = FetchEncodedPixelArray(*encodedImage, encodedImageStream);
        std::vector<unsigned char> decodedPixelVec = DecodePixelArray(encodedPixelArray, *encodedImage, deserializedRootNode);

        cimg_library::CImg<unsigned char> rawImg = CreateDecodedImage(decodedPixelVec, *encodedImage, currentFile);
        CreateImageFile(rawImg, currentFile, marshaller, *encodedImage);
    }

}

std::ifstream HuffmanDecodingStrategy::GetDecodedFileStream(File& currentFile, FileMarshaller& marshaller){
    return marshaller.CreateInfileStream(currentFile.fullPath, std::ifstream::binary);
}

void HuffmanDecodingStrategy::AdjustStreamPointer(std::ifstream& encodedFileStream, int pointerStart){
    encodedFileStream.seekg(pointerStart);
}

std::shared_ptr<HuffmanTreeNode> HuffmanDecodingStrategy::DeserializeFileData(std::ifstream& encodedFileStream){
    std::shared_ptr<HuffmanTreeNode> tmpRoot;
    {
        cereal::BinaryInputArchive binaryInputArchive(encodedFileStream);
        binaryInputArchive(tmpRoot);
    }
    return tmpRoot;
}

unsigned char* HuffmanDecodingStrategy::FetchEncodedPixelArray(JCIFImage& encodedImage, std::ifstream& encodedFileStream){
    unsigned char* encodedPixelArray = new unsigned char[encodedImage.encodedPixelArrayBytes];
    ReadBytesInto(encodedFileStream, encodedPixelArray, encodedImage.encodedPixelArrayBytes);
    return encodedPixelArray;
}

void HuffmanDecodingStrategy::ReadBytesInto(std::ifstream& encodedFileStream, unsigned char* encodedPixelArray, int bytesToRead){
    encodedFileStream.read((char*) encodedPixelArray, bytesToRead);
}

std::vector<unsigned char> HuffmanDecodingStrategy::DecodePixelArray(unsigned char* encodedPixelArray, JCIFImage& encodedImage, std::shared_ptr<HuffmanTreeNode> rootNode){
    std::shared_ptr<HuffmanTreeNode> currentNode = rootNode;
    std::vector<unsigned char> decodedPixelVec;

    int encodedPixelArrayBits = (encodedImage.encodedPixelArrayBytes*8) - encodedImage.bitPadding;
    int byteIndex = 0;
    int bitIndex = 0;
    int bits = 0;

    unsigned char currentByte = encodedPixelArray[byteIndex++];
    int nextCode;
    int msb;
    //Bit Manipulator
    while(bits < encodedPixelArrayBits){
        if(bitIndex >= 8){
            currentByte = encodedPixelArray[byteIndex++];
            bitIndex = 0;
        }

        if(currentNode->left == nullptr && currentNode->right == nullptr){
            decodedPixelVec.push_back(currentNode->pix);
            currentNode = rootNode;
        }

        msb = 7-bitIndex++;
        nextCode = (currentByte >> msb) & 1;
        if((nextCode ^ 0) == 0){
            currentNode = currentNode->left;
        }
        else{
            currentNode = currentNode->right;
        }
        bits++;        
    }

    return decodedPixelVec;
}

cimg_library::CImg<unsigned char> HuffmanDecodingStrategy::CreateDecodedImage(std::vector<unsigned char>& decodedPixelVec, JCIFImage& encodedImage, File& currentFile){
    cimg_library::CImg<unsigned char> newImage(decodedPixelVec.data(), encodedImage.header->imageWidth, encodedImage.header->imageHeight, 1, 3); 
    return newImage;
}   

void HuffmanDecodingStrategy::CreateImageFile(cimg_library::CImg<unsigned char> img, File& currentFile, FileMarshaller& marshaller, JCIFImage& encodedImage){
    marshaller.FlagFileForCleanUp(currentFile.fullPath);
    switch(encodedImage.originalFileType){
        case File::FileType::BMP:
            currentFile.fullPath =  currentFile.relativePath + "/" + currentFile.name +".bmp";
            currentFile.type = File::FileType::BMP;
            currentFile.ext = "bmp";
             
            img.save_bmp(currentFile.fullPath.c_str());
            break;
        case File::FileType::TIFF:
            currentFile.fullPath =  currentFile.relativePath + "/" + currentFile.name +".tiff";
            currentFile.type = File::FileType::TIFF;
            currentFile.ext = "tiff";

            img.save_tiff(currentFile.fullPath.c_str());
            break;
    }
}