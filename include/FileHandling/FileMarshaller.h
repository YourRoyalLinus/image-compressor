#ifndef FILEMARSHALLER_H
#define FILEMARSHALLER_H

#include "./FileObjects/File.h"
#include <vector>
#include <ios>


class FileMarshaller{
    public:
        File* InitializeFile(std::string filePath);

        void ParseFile(File& file);
        void ConvertFileToBMP(File& file);
        void ConvertFileToTIFF(File& file);
        void UpdateFilePath(std::string newPath, File& file);
        void UpdateFileExt(File& file, std::string newExt);

        long GetFileSize(const File& file);

        std::string CreateLocalCopy(std::string localDirPath, File& file);
        std::string CreatePath(std::string src, std::string path);
        std::string CreateHomePath();

        std::ofstream CreateOutfileStream(std::string encodedFilePath, std::ios_base::openmode mode);
        std::ifstream CreateInfileStream(std::string encodedFilePath, std::ios_base::openmode mode);

        bool DoesPathExist(std::string filePath);
        bool IsValidFileType(std::string fileExt);
        
        void FlagFileForCleanUp(std::string filePath);
        void CleanUpTempFiles();
    private:
        std::vector<std::string> _cleanUpFiles;
};

#endif