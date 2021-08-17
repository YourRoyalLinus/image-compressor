#include "../include/FileSystem.h"
#include "../include/Utils.h"

#include <iostream>
#include <fstream>
#include <pwd.h>
#include <experimental/filesystem>

FileSystem::FileSystem(){

}

FileSystem& FileSystem::instance(){
    static FileSystem *instance = new FileSystem();
    return *instance;
}
        
std::string FileSystem::CreatePath(const std::string& srcPath, const std::string& destPath){ 
    std::string newPath = srcPath + "/" + destPath;
    if (DoesPathExist(newPath)) return newPath;
    std::experimental::filesystem::create_directory(newPath);
    if (!DoesPathExist(newPath)) {
        std::cout << "Error while creating " << destPath << std::endl; 
        exit(1);
    }
    return newPath;
}


std::string FileSystem::FindUserPath(){
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    return std::string(homedir);
} 

void FileSystem::CopyFileContents(const std::string& srcFile, const std::string& destFile){
    std::experimental::filesystem::copy(srcFile, destFile);
}

void FileSystem::DeleteFile(const std::string& filePath){
    std::error_code ec;
    bool r = std::experimental::filesystem::remove(filePath, ec);
    if(!r){
        std:: cout << "Error deleting file: " << filePath << " - " << ec.message() << std::endl;
    }
} 

long FileSystem::GetFileSize(const std::string& filePath){
    std::experimental::filesystem::path path = filePath;
    return std::experimental::filesystem::file_size(path);
    
}

bool FileSystem::DoesPathExist(const std::string& filePath){
    return std::experimental::filesystem::exists(filePath);
}

bool FileSystem::IsValidFileType(std::string fileExt){
    if(find(validFileTypes, validFileTypes+countOfValidFileTypes-1, fileExt) == validFileTypes+countOfValidFileTypes){
        std::cout << "\"" << fileExt << "\" is not one of the supported file types\n";
        return false;
    }

    return true;
}