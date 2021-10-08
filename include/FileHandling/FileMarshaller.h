#ifndef FILEMARSHALLER_H
#define FILEMARSHALLER_H

#include "./FileObjects/File.h"
#include <ios>

class FileMarshaller{
    public:
        File* InitializeFile(std::string filePath);
        void CleanupFileResources(File& file);

        void ParseFile(File& file);
        void ConvertFileToBMP(File& file);
        void ConvertFileToTIFF(File& file);
        void UpdateFilePath(std::string newPath, File& file);

        long GetFileSize(const File& file);

        std::string CreateLocalCopy(std::string localDirPath, File& file);
        std::string CreatePath(std::string src, std::string path);
        std::string CreateHomePath();

        std::ofstream CreateOutfileStream(std::string encodedFilePath, std::ios_base::openmode mode);
        std::ifstream CreateInfileStream(std::string encodedFilePath, std::ios_base::openmode mode);

        bool DoesPathExist(std::string filePath);
        bool IsValidFileType(std::string fileExt);

        void WriteFileToDisk(File& file); //Poly between Encoded and Decoded Files
        void ReadFileFromDisk(File& file); //Poly between Encoded and Decoded Files
        
        void CleanUpTempFiles();
};

#endif