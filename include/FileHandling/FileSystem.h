#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <fstream>
#include <memory>

//TODO throws errors?
class FileSystem{
    public:
        static FileSystem& instance();
        
        std::string CreatePath(const std::string& srcPath, const std::string& destPath); //need exits on fail opens
        std::string FindUserPath();
        std::string GetFileExt(const std::string& filePath);
        std::shared_ptr<std::ofstream> CreateOutfileStream(const std::string& filePath, std::ios_base::openmode mode);
        std::shared_ptr<std::ifstream> CreateInfileStream(const std::string& filePath, std::ios_base::openmode mode);
        void CopyFileContents(const std::string& srcFile, const std::string& destFile);
        void DeleteFile(const std::string& filePath);
        long GetFileSize(const std::string& stringPath);
        bool DoesPathExist(const std::string& filePath);
        
    protected:
        FileSystem();
};

#endif