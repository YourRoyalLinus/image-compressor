#include "../../include/FileHandling/FileSystem.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <experimental/filesystem>
#include <pwd.h>
#include <unistd.h>

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

std::string FileSystem::GetFileExt(const std::string& filePath){
    return std::experimental::filesystem::path(filePath).extension();
}

std::shared_ptr<std::ofstream> FileSystem::CreateOutfileStream(const std::string& filePath, std::ios_base::openmode mode){
    std::shared_ptr<std::ofstream> f(new std::ofstream(filePath, mode));
    if (!f->is_open())
    {
        std::cout << "Error opening file" << filePath << std::endl;
        exit(-1);
    }

    return f;
}

std::shared_ptr<std::ifstream>FileSystem::CreateInfileStream(const std::string& filePath, std::ios_base::openmode mode){
    std::shared_ptr<std::ifstream> f(new std::ifstream(filePath, mode));
    if (!f->is_open())
    {
        std::cout << "Error opening file" << filePath << std::endl;
        exit(-1);
    }

    return f;
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