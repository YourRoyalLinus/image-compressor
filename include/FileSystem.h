#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

//TODO throws errors?
class FileSystem{
    public:
        static FileSystem& instance();
        
        std::string CreatePath(const std::string& srcPath, const std::string& destPath);
        std::string FindUserPath();
        void CopyFileContents(const std::string& srcFile, const std::string& destFile);
        void DeleteFile(const std::string& filePath);
        long GetFileSize(const std::string& stringPath);
        bool DoesPathExist(const std::string& filePath);
        bool IsValidFileType(std::string fileExt);

        inline static std::string validFileTypes[3] = { "bmp", "tif", "tiff" };
        static const unsigned int countOfValidFileTypes = 3;
    protected:
        FileSystem();
};

#endif