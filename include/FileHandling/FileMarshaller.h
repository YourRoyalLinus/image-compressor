#ifndef FILEMARSHALLER_H
#define FILEMARSHALLER_H

#include "./FileObjects/File.h"
#include <memory>
#include <vector>
#include <ios>


class FileMarshaller{
    public:
        static FileMarshaller& instance();
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

        std::shared_ptr<std::ofstream> CreateOutfileStream(std::string encodedFilePath, std::ios_base::openmode mode);
        std::shared_ptr<std::ifstream> CreateInfileStream(std::string encodedFilePath, std::ios_base::openmode mode);

        bool DoesPathExist(std::string filePath);
        bool IsValidFile(File& file);
        
        void FlagFileForCleanUp(std::string filePath);
        void CleanUpTempFiles();
    protected:
        FileMarshaller();
    private:
        std::vector<std::string> _cleanUpFiles;
};

#endif