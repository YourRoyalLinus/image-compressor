#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "./BaseImage.h"
#include "../../FileHandling/FileObjects/FileHeader.h"
#include "../../../lib/CImg-2.9.9/CImg.h"


struct BMPImage : public BaseImage{
    public:

        BMPImage(const char* filePath) : BaseImage(filePath){
        }  

        unsigned char* pixelDataArray;   
        cimg_library::CImg<unsigned char> cimage;
};

#endif