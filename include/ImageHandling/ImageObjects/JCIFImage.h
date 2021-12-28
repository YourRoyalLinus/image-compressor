#ifndef JCIFIMAGE_H
#define JCIFIMAGE_H

#include "./BaseImage.h"
#include "../../FileHandling/FileObjects/File.h"


struct JCIFImage : public BaseImage{
    public:

        JCIFImage(const char* filePath) : BaseImage(filePath){
        }  

        unsigned short int bitPadding;
        unsigned int encodedPixelArrayBytes;
        File::FileType originalFileType;
};

#endif