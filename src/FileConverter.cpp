#include "FileConverter.h"
#include "Utils.h"
#include <bitset>
#include <vector>

FileConverter::FileConverter(){
    cimg_library::cimg::imagemagick_path(imageMagickPath.c_str());
}

FileConverter::~FileConverter(){
}

void FileConverter::ConvertFileToBMP(FileInfo* fi){ 
    cimg_library::CImg<unsigned char> image(fi->fullFilePath.c_str());
    switch(fi->type){
        case FileConverter::FileType::BMP:
            fi->bmpPath = fi->fullFilePath;
            break;
        default:
            std::string tmp = fi->relativeFilePath + fi->fileName +".bmp"; 
            image.save(tmp.c_str());
            fi->bmpPath = tmp;
            break;
    }
}

long FileConverter::SaveEncodedDataFile(FileInfo* fi){
    int height = 0;
    int width = 0;
    long offset = 0;
    int fileSize = 0;
    short int bpp = 0;
    unsigned char header[54];

    FILE* imgFile = fopen(fi->encodedPath.c_str(), "rb");

    if (imgFile == nullptr)
    {
        std::cout << "Error opening tmp file" << fi->encodedPath << std::endl;
        return -1;
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

    return fileSize;
}

struct FileConverter::FileInfo* FileConverter::ParseFile(const std::string& file){
    struct FileInfo* fi = new FileInfo();

    fi->fullFilePath = file;
    fi->initialSize = Utils::GetFileSize(file);

    std::vector<std::string> pathTokens = Utils::StringSplit(file, '/');
    for(unsigned i = 0; i < pathTokens.size()-1; i++){
        fi->relativeFilePath += pathTokens[i] + "/";
    }

    std::vector<std::string> fileNameExt = Utils::StringSplit(pathTokens[pathTokens.size()-1], '.');

    fi->ext = fileNameExt[fileNameExt.size()-1];
    fi->type = extTypeMap.find(fi->ext)->second;

    if(fi->type == FileType::BMP){
        fi->converted = false;
    }
    else{
        fi->converted = true;
    }

    fi->fileName = fileNameExt[0];
    
    for(unsigned i = 1; i < fileNameExt.size()-1; i++){
        fi->fileName += "." + fileNameExt[i];
    }

    fi->compressedSize = -1;
    fi->encodedSize = -1;

    return fi;
}