#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "../FileHandling/FileObjects/FileHeader.h"
#include "../Utils/Uncopyable.h"
#include "../../lib/CImg-2.9.9/CImg.h"
#include <memory>


struct BMPImage : private Uncopyable{
    public:

        BMPImage(const char* filePath){
            path = filePath;
        }  

        const char* path;
        unsigned char* pixelDataArray;   
        cimg_library::CImg<unsigned char> cimage;
        std::unique_ptr<FileHeader> header;
};

#endif