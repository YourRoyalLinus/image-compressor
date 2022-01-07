#include "../../../include/FileHandling/FileObjects/FileMetadata.h"
#include "../../../include/Utils/GeneralUtility.h"

FileMetadata::FileMetadata(std::string fullFilePath){
    GetPathTokens(fullFilePath);
    GetPathComponents();
}

void FileMetadata::GetPathTokens(std::string fullFilePath){
    pathTokens = Utility::StringSplit(fullFilePath, '/');
}

void FileMetadata::GetPathComponents(){
    unsigned int numberOfPathTokens = pathTokens.size();
    pathComponents = Utility::StringSplit(pathTokens[numberOfPathTokens-1], '.');
}
