#include "../../include/ImageHandling/ImageParser.h"
#include <fstream>
#include <iostream>

ImageParser::ImageParser(){
}

ImageParser& ImageParser::instance(){
    static ImageParser *instance = new ImageParser();
    return *instance;
}

void ImageParser::ParseImage(BMPImage& img){
    FetchFileHeaderData(img);
    CreateCImage(img);
}

void ImageParser::ParseImage(JCIFImage& img){
   FetchFileHeaderData(img);
   img.originalFileType = (File::FileType) img.header->reservedByteOne;
   img.bitPadding = img.header->reservedByteTwo;
   img.encodedPixelArrayBytes = img.header->imageSize - img.header->pixelDataOffset;
}

void ImageParser::FetchFileHeaderData(BaseImage& img){
    unsigned char headerBuffer[this->headerSize];

    {
        std::ifstream fileStream(img.path);
        if(!fileStream.is_open()){
            std::cout << "Error opening " << img.path << std::endl;
            return;
        }
        
        fileStream.readsome((char*)headerBuffer, this->headerSize);
    }

    img.header = std::unique_ptr<FileHeader>(new FileHeader(headerBuffer));
}


void ImageParser::CreateCImage(BMPImage& img){
    cimg_library::CImg<unsigned char> i(img.path);
    img.cimage = i;
    FetchPixelDataArray(img);
}

void ImageParser::FetchPixelDataArray(BMPImage& img){
    img.pixelDataArray = img.cimage.data();
}