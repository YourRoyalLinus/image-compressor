#include "../../include/FileHandling/FileConverter.h"
#include "../../include/FileHandling/FileMarshaller.h"
#include "../../include/FileHandling/FileSystem.h"
#include "../../include/FileHandling/FileParser.h"

File* FileMarshaller::InitializeFile(std::string filePath){
    File* tmp = new File(filePath);
    return tmp;
}

void FileMarshaller::ParseFile(File& file){
    FileParser::instance().ParseFile(file);
}

void FileMarshaller::ConvertFileToBMP(File& file){
    FileConverter::instance().ConvertFileToBMP(file);
}

void FileMarshaller::ConvertFileToTIFF(File& file){
    FileConverter::instance().ConvertFileToTIFF(file);
}

void FileMarshaller::UpdateFilePath(std::string newPath, File& file){
    file.relativePath = newPath;
    file.fullPath = newPath + "/" + file.name + "." + file.ext;
}

void FileMarshaller::UpdateFileExt(File& file, std::string newExt){
    file.ext = newExt;
    file.fullPath = file.relativePath + "/" + file.name + "." + file.ext;
}

long FileMarshaller::GetFileSize(const File& file){
    return FileSystem::instance().GetFileSize(file.fullPath);
}

std::string FileMarshaller::CreateLocalCopy(std::string localDirPath, File& file){
    std::string destPath = localDirPath + "/" + file.name + "." + file.ext;
    FileSystem::instance().CopyFileContents(file.fullPath, destPath);
    return destPath;
}

std::string FileMarshaller::CreatePath(std::string src, std::string path){
    return FileSystem::instance().CreatePath(src, path);
}

std::string FileMarshaller::CreateHomePath(){
    std::string usrPath = FileSystem::instance().FindUserPath();
    std::string homePath = usrPath +"/ImageCompressor";

    if(!FileSystem::instance().DoesPathExist(homePath)){
        homePath = FileSystem::instance().CreatePath(usrPath, "ImageCompressor");
    }

    return homePath;
}

std::ofstream FileMarshaller::CreateOutfileStream(std::string encodedFilePath, std::ios_base::openmode mode){
    return FileSystem::instance().CreateOutfileStream(encodedFilePath, mode);
}

std::ifstream FileMarshaller::CreateInfileStream(std::string encodedFilePath, std::ios_base::openmode mode){
    return FileSystem::instance().CreateInfileStream(encodedFilePath, mode);
}

bool FileMarshaller::DoesPathExist(std::string filePath){
    return FileSystem::instance().DoesPathExist(filePath);
}

bool FileMarshaller::IsValidFileType(std::string fileExt){
    return FileSystem::instance().IsValidFileType(fileExt);
}

void FileMarshaller::FlagFileForCleanUp(std::string filePath){
    _cleanUpFiles.push_back(filePath);
}

void FileMarshaller::CleanUpTempFiles(){
    for(auto _filePath : _cleanUpFiles){
        FileSystem::instance().DeleteFile(_filePath);
    }
}