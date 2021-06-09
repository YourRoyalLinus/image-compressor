#include "FileConverter.h"
#include "Utils.h"
#include <bitset>
#include <vector>

FileConverter::FileConverter(){   
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
            fi->bmpPath = tmp;
            break;
    }
}
void FileConverter::ConvertFileToOriginal(FileInfo* fi){
    std::string tmp;
    
    int height = 0;
    int width = 0;
    long offset = 0;
    long fileSize = 0;
    char buffer[32];

    FILE* imgFile = fopen(fi->tmpPath.c_str(), "rb");

    if (imgFile == nullptr)
    {
        std::cout << "Error opening tmp file" << fi->tmpPath << std::endl;
        return;
    }

    fseek(imgFile, 16, SEEK_SET);
    fread(&buffer, 1, 32, imgFile);
    fileSize = std::bitset<32>(buffer).to_ulong();

    fseek(imgFile, 80, SEEK_SET);
    fread(&buffer, 1, 32, imgFile);
    offset = std::bitset<32>(buffer).to_ulong();

    fseek(imgFile, 144, SEEK_SET);
    fread(&buffer, 1, 32, imgFile);
    width = std::bitset<32>(buffer).to_ulong();
    fread(&buffer, 1, 32, imgFile);
    height = std::bitset<32>(buffer).to_ulong();
    
    fseek (imgFile , offset, SEEK_END);
    int imgSize = ftell (imgFile);
    rewind (imgFile);

    char* buff = new char[imgSize+1];
    fread (buff,1,imgSize,imgFile);
    buff[imgSize] = '\0';
    
    /*
    char* imgBuffer = new char[(fileSize-offset+1)];

    fseek(imgFile, offset, SEEK_SET);
    fread(&imgBuffer, (fileSize-offset), 1, imgFile);

    imgBuffer[fileSize] = '\0';
    */
    cimg_library::CImg<unsigned char> image(buff, width, height, 1, 3);

    //cimg_library::CImg<unsigned char> image;
    //image.load_bmp(fi->tmpPath.c_str());
    switch(fi->converted){
        case true:
            tmp = fi->relativeFilePath + fi->fileName + fi->ext; 
            switch(fi->type){
                case FileType::JPEG:
                    image.save_jpeg(tmp.c_str());
                    break;
                case FileType::PNG:
                    image.save_png(tmp.c_str());
                    break;
                case FileType::TIFF:
                    image.save_tiff(tmp.c_str());
                    break;
            }
            fi->fullFilePath = tmp;
            break;
        case false:
            image.save_bmp(fi->fullFilePath.c_str());
            break;
    }  
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

    return fi;
}