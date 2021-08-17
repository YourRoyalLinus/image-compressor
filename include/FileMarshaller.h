#ifndef FILEMARSHALLER_H
#define FILEMARSHALLER_H

#include "../include/File.h"

class FileMarshaller{
    public:
        File* InitializeFile(std::string filePath);
        void CleanupFileResources(File& file);

        void ParseFile(File& file);
        void ConvertFileToBMP(File& file);
        void UpdateFilePath(std::string newPath, File& file);

        long GetFileSize(const File& file);

        std::string CreateLocalCopy(std::string localDirPath, File& file);
        std::string CreateHomePath();
        std::string CreatePath(std::string src, std::string path);
        
        bool DoesPathExist(std::string filePath);
        bool IsValidFileType(std::string fileExt);

        void WriteFileToDisk(File& file); //Poly between Encoded and Decoded Files
        void ReadFileFromDisk(File& file); //Poly between Encoded and Decoded Files
        
        void CleanUpTempFiles();
};

#endif