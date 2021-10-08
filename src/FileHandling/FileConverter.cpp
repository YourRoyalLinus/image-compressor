#include "../../include/FileHandling/FileConverter.h"

FileConverter::FileConverter(){
    cimg_library::cimg::imagemagick_path(imageMagickPath.c_str()); //TODO dynamic config?
}

FileConverter& FileConverter::instance(){
        static FileConverter *instance = new FileConverter();
        return *instance;
}

void FileConverter::ConvertFileToBMP(File& file){ 
    cimg_library::CImg<unsigned char> image(file.fullPath.c_str());
    switch(file.type){
        case File::FileType::BMP:
            break;
        default:
            file.fullPath =  file.relativePath + "/" + file.name +".bmp";
            file.type = File::FileType::BMP;
            file.ext = "bmp";
                
            image.save_bmp(file.fullPath.c_str());    
            break;
    }           
}

void FileConverter::ConvertFileToTIFF(File& file){
    cimg_library::CImg<unsigned char> image(file.fullPath.c_str());
    switch(file.type){
        case File::FileType::TIFF:
            break;
        default:
            file.fullPath =  file.relativePath + "/" + file.name +".tiff";
            file.type = File::FileType::TIFF;
            file.ext = "tiff";
            
            image.save_tiff(file.fullPath.c_str());    
            break;
    }  
}