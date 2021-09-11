#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "./FileObjects/File.h"
#include "./FileObjects/FileMetadata.h"
#include "./FileSystem.h"
#include <unordered_map>
#include <sys/stat.h>


class FileParser{
    public:
        static FileParser& instance();
        void ParseFile(File& file);

    protected:
        FileParser();
    private:
        void GetFileSize(File& file);
        void GetRelativePath(File& file, const FileMetadata& FileMetadata);
        void GetFileName(File& file,  const FileMetadata& FileMetadata);
        void GetFileExt(File& file,  const FileMetadata& FileMetadata);
        void GetFileType(File& file);


        inline static std::unordered_map<std::string, File::FileType> extTypeMap = { {"bmp", File::FileType::BMP}, {"tif", File::FileType::TIFF}, {"tiff", File::FileType::TIFF} };
        
};

#endif