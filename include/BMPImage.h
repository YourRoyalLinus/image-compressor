#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "FileHeader.h"
#include "Artifact.h"
#include "CImg/CImg-2.9.8_pre051821/CImg.h"

class BMPImage : public Artifact{
    public:

        BMPImage(const char* filePath){
            path = filePath;
            type = Artifact::ArtifactType::BMPIMAGE;
        }  

        ~BMPImage(){
            if(header != nullptr){
                delete(header);
                header = nullptr;
            }
        }

        const char* path;
        unsigned char* pixelDataArray;   
        cimg_library::CImg<unsigned char> cimage;
        FileHeader* header;
};

#endif