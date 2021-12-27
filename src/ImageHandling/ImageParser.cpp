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

void ImageParser::FetchFileHeaderData(BMPImage& img){
    const unsigned int headerSize = 54;
    unsigned char headerBuffer[headerSize];

    
    {
        std::ifstream fileStream(img.path);
        if(!fileStream.is_open()){
            std::cout << "Error opening " << img.path << std::endl;
            return;
        }
        
        fileStream.readsome((char*)headerBuffer, headerSize);
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