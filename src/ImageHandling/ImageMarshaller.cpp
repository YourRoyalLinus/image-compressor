#include "../../include/ImageHandling/ImageObjects/BMPImage.h"
#include "../../include/ImageHandling/ImageObjects/JCIFImage.h"
#include "../../include/ImageHandling/ImageMarshaller.h"
#include "../../include/ImageHandling/ImageParser.h"
#include "../../include/FileHandling/FileConverter.h"
#include <memory>
#include <iostream>

ImageMarshaller::ImageMarshaller(){
}

ImageMarshaller& ImageMarshaller::instance(){
    static ImageMarshaller *instance = new ImageMarshaller();
    return *instance;
}

void ImageMarshaller::ParseImage(std::shared_ptr<BaseImage> img){
    if(std::dynamic_pointer_cast<BMPImage>(img) != nullptr){
        std::shared_ptr<BMPImage> bmp = std::dynamic_pointer_cast<BMPImage>(img);
        ImageParser::instance().ParseImage(*bmp);
    }
    else if(std::dynamic_pointer_cast<JCIFImage>(img) != nullptr){
        std::shared_ptr<JCIFImage> jcif = std::dynamic_pointer_cast<JCIFImage>(jcif);
        ImageParser::instance().ParseImage(*jcif);
    }
    else{
        std::cout << "Attempted to parse an invalid image" << std::endl;
        exit(-1);
    }
    
}

void ImageMarshaller::SaveImage(File& currentFile, JCIFImage& encodedImage, cimg_library::CImg<unsigned char>& newImage){
    switch(encodedImage.originalFileType){
        case File::FileType::BMP:
            FileConverter::instance().ConvertImageToBMP(currentFile, newImage);
            break;
        case File::FileType::TIFF:
            FileConverter::instance().ConvertImageToTIFF(currentFile, newImage);
            break;
        default:
            std::cout << "Error decoding image: " << encodedImage.path << std::endl;
            exit(-1);
            break;
    }
}