#include "../../../include/FileHandling/FileObjects/FileMetadata.h"
#include "../../../include/Utils/Utils.h"

FileMetadata::FileMetadata(std::string fullFilePath){
    GetPathTokens(fullFilePath);
    GetPathComponents();
}

void FileMetadata::GetPathTokens(std::string fullFilePath){
    pathTokens = Utils::StringSplit(fullFilePath, '/');
}

void FileMetadata::GetPathComponents(){
    unsigned int numberOfPathTokens = pathTokens.size();
    pathComponents = Utils::StringSplit(pathTokens[numberOfPathTokens-1], '.');
}
