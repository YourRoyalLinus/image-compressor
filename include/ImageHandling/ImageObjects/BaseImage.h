#ifndef BASEIMAGE_H
#define BASEIMAGE_H

#include "../../FileHandling/FileObjects/FileHeader.h"
#include "../../Utils/Uncopyable.h"
#include <memory>


struct BaseImage : private Uncopyable{
    public:

        BaseImage(const char* filePath){
            path = filePath;
        }  
        virtual ~BaseImage(){
        }
        
        const char* path;
        std::unique_ptr<FileHeader> header;
};

#endif