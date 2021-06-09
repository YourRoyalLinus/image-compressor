#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <unordered_map>
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
//Reqiures CIMG.h
//Requires ImageMagik

class FileConverter{ //FileManager?
    public:
        enum class FileType{
            BMP,
            JPEG,
            PNG,
            TIFF
        };
        struct FileInfo{
            std::string tmpPath;
            std::string bmpPath;
            std::string relativeFilePath;
            std::string fullFilePath;
            std::string fileName;
            FileType type;
            std::string ext;
            long initialSize;
            long compressedSize;
            bool converted;
        };
        FileConverter();
        ~FileConverter();
        
        void ConvertFileToBMP(FileInfo* fi);
        void ConvertFileToOriginal(FileInfo* fi);
        struct FileInfo* ParseFile(const std::string& file);


    private:
        const std::unordered_map<std::string, FileType> extTypeMap = { {"bmp", FileType::BMP}, {"jpg", FileType::JPEG}, {"jpeg", FileType::JPEG},
        {"png", FileType::PNG}, {"tif", FileType::TIFF}, {"tiff", FileType::TIFF} }; 
};

#endif