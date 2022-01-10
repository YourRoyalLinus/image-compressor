#include "../../include/FileHandling/FileParser.h"
#include <algorithm>
#include <iostream>
FileParser::FileParser(){

}

FileParser& FileParser::instance(){
    static FileParser *instance = new FileParser();
    return *instance;
}

void FileParser::ParseFile(File& f){
    FileMetadata metaData(f.fullPath);
    GetFileSize(f);
    GetRelativePath(f, metaData);
    GetFileName(f, metaData);
    GetFileExt(f, metaData);

}

void FileParser::GetFileSize(File& f){
    f.size = FileSystem::instance().GetFileSize(f.fullPath);
}

void FileParser::GetRelativePath(File& f, const FileMetadata& fileMetadata){
    for(unsigned i = 0; i < fileMetadata.pathTokens.size()-1; i++){
        f.relativePath += fileMetadata.pathTokens[i] + "/";
    }
}

void FileParser::GetFileName(File& f, const FileMetadata& fileMetadata){
    unsigned int numberOfPathComponents = fileMetadata.pathComponents.size();
    std::string fileNameBeforeFirstPeriod = fileMetadata.pathComponents[0];
    std::string fileNameAfterFirstPeriod = "";
    for(unsigned i = 1; i < numberOfPathComponents-1; i++){
        //fileNameAfterMultiplePeriods will be empty if there are no periods '.' other than the f extension delimeter
        fileNameAfterFirstPeriod += "." + fileMetadata.pathComponents[i];
    }

    f.name = fileNameBeforeFirstPeriod + fileNameAfterFirstPeriod;    
}

void FileParser::GetFileExt(File& f, const FileMetadata& fileMetadata){
    unsigned int numberOfPathComponents = fileMetadata.pathComponents.size();
    f.ext = fileMetadata.pathComponents[numberOfPathComponents-1];

    std::transform(f.ext.begin(), f.ext.end(), f.ext.begin(),
                    [](unsigned char c){ return std::tolower(c); });

    GetFileType(f);
    IsCompressed(f);
}

void FileParser::GetFileType(File& f){
    std::unordered_map<std::string, File::FileType>::iterator iter = extTypeMap.find(f.ext);
    if(iter == extTypeMap.end()){
        f.type = File::FileType::INVALID;
    }
    else{
        f.type = iter->second;
    }
    
}

void FileParser::IsCompressed(File& f){
    f.isEncoded = (f.type == File::FileType::JCIF);
}








