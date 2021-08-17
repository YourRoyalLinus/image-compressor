#include "../include/FileMarshaller.h"
#include "../include/FileSystem.h"
#include "../include/FileConverter.h"
#include "../include/FileParser.h"

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

void FileMarshaller::UpdateFilePath(std::string newPath, File& file){
    file.relativePath = newPath;
    file.fullPath = newPath + "/" + file.name + "." + file.ext;
}

long FileMarshaller::GetFileSize(const File& file){
    return FileSystem::instance().GetFileSize(file.fullPath);
}

std::string FileMarshaller::CreateHomePath(){
    std::string usrPath = FileSystem::instance().FindUserPath();
    std::string homePath = usrPath +"/ImageCompressor";

    if(!FileSystem::instance().DoesPathExist(homePath)){
        homePath = FileSystem::instance().CreatePath(usrPath, "ImageCompressor");
    }

    return homePath;
}

std::string FileMarshaller::CreatePath(std::string src, std::string path){
    return FileSystem::instance().CreatePath(src, path);
}


std::string FileMarshaller::CreateLocalCopy(std::string localDirPath, File& file){
    std::string destPath = localDirPath + "/" + file.name + "." + file.ext;
    FileSystem::instance().CopyFileContents(file.fullPath, destPath);
    return destPath;
}

bool FileMarshaller::DoesPathExist(std::string filePath){
    return FileSystem::instance().DoesPathExist(filePath);
}

bool FileMarshaller::IsValidFileType(std::string fileExt){
    return FileSystem::instance().IsValidFileType(fileExt);
}


void FileMarshaller::WriteFileToDisk(File& file){
    int height = 0;
    int width = 0;
    long offset = 0;
    int fileSize = 0;
    short int bpp = 0;
    unsigned char header[54];

    FILE* imgFile = fopen(file.fullPath.c_str(), "rb");

    if (imgFile == nullptr)
    {
        //std::cout << "Error opening tmp file" << file.fullPath << std::endl;
        return;
    }

    // Reader header data into the header array
    fread(header, sizeof(unsigned char), 54, imgFile);
    
    fileSize = *(int*)&header[2];
    offset = *(int*)&header[10];
    width = *(int*)&header[18];
    height = *(int*)&header[22];
    bpp = *(int*)&header[28];

    // Move stream pointer to the start of the data 
    fseek(imgFile, offset, SEEK_SET);
    int pixelDataSize = fileSize - offset;
    unsigned char* pixelArray = new unsigned char[pixelDataSize+1];
    fread(pixelArray, sizeof(unsigned char), pixelDataSize, imgFile);
    pixelArray[pixelDataSize] = '\0';

}


