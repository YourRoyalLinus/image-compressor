#ifndef FILE_H
#define FILE_H

#include <string>

class File{
    public:
        enum FileType{
            BMP,
            TIFF,
            JCIF
        };

        File(std::string filePath){
            fullPath = filePath;
        }

        long size;
        bool isEncoded;
        std::string fullPath;
        std::string relativePath;
        std::string name;
        std::string ext;

        FileType type;
};

#endif