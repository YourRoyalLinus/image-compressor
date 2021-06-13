#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <unordered_map>
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
//Requires ImageMagik

class FileConverter{ 
    public:
        enum class FileType{
            BMP,
            JPEG,
            PNG,
            TIFF
        };
        struct FileInfo{
            std::string encodedPath;
            std::string bmpPath;
            std::string tmpPath;
            std::string relativeFilePath;
            std::string fullFilePath;
            std::string fileName;
            FileType type;
            std::string ext;
            long initialSize;
            long compressedSize;
            long encodedSize;
            bool converted;
        };
        FileConverter();
        ~FileConverter();
        
        void ConvertFileToBMP(FileInfo* fi);
        long SaveEncodedDataFile(FileInfo* fi);
        struct FileInfo* ParseFile(const std::string& file);

    private:
        const std::string imageMagickPath = "/usr/local/bin/convert";
        const std::unordered_map<std::string, FileType> extTypeMap = { {"bmp", FileType::BMP}, {"jpg", FileType::JPEG}, {"jpeg", FileType::JPEG},
        {"png", FileType::PNG}, {"tif", FileType::TIFF}, {"tiff", FileType::TIFF} }; 
};

#endif