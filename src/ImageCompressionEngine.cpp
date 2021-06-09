#include "ImageCompressionEngine.h"
#include "Utils.h"
#include <chrono>

ImageCompressionEngine::ImageCompressionEngine() : fileConverter(new FileConverter()), imageCompressor(new ImageCompressor()){
}

ImageCompressionEngine::ImageCompressionEngine(std::string iDir, std::string oDir, std::string tsDir, std::vector<std::string> ifiles) : inboundDir(iDir),
outboundDir(oDir), timestampDir(tsDir), inputFiles(ifiles), fileConverter(new FileConverter()), imageCompressor(new ImageCompressor()){
}

ImageCompressionEngine::~ImageCompressionEngine(){
    delete fileConverter;
    delete imageCompressor;
}

std::string ImageCompressionEngine::CreateLocalInboundCopy(){
    std::string destPath = inboundDir + "/" + fileInfo->fileName + "." + fileInfo->ext;
    std::experimental::filesystem::copy(fileInfo->fullFilePath, destPath);
    currentFile = fopen(destPath.c_str(), "r");
    if(currentFile == nullptr){ //IF BRANCH NEEDED??
        return "";
    }
    else{
        fclose(currentFile);
        return destPath;
    }
}

std::string ImageCompressionEngine::CreateLocalOutboundCopy(){
    std::string destPath = outboundDir + "/" + fileInfo->fileName + "." + fileInfo->ext; //Should be overwritten by compressed file!
    std::experimental::filesystem::copy(fileInfo->fullFilePath, destPath);
    currentFile = fopen(destPath.c_str(), "r");
    if(currentFile == nullptr){ //IF BRANCH NEEDED??
        return "";
    }
    else{
        fclose(currentFile);
        return destPath;
    }
}

int ImageCompressionEngine::StartBatchCompression(){
    int inFiles = inputFiles.size();
    int outFiles = 0;

    for(unsigned i = 0; i < inputFiles.size(); i++){
        try{
            executeStart = std::chrono::system_clock::now();
            std::string f = inputFiles[i];

            fileInfo = fileConverter->ParseFile(f);
            std::string inboundFile = CreateLocalInboundCopy();
            std::string outboundFile = CreateLocalOutboundCopy();

            if(inboundFile.length() == 0 || outboundFile.length() == 0){
                std::cout << "Error opening file: " << fileInfo->fileName + "." + fileInfo->ext << std::endl;
                continue;
            }

            fileInfo = fileConverter->ParseFile(outboundFile);
            fileConverter->ConvertFileToBMP(fileInfo);
            //Compress image
            imageCompressor->CompressImageFile(fileInfo);

            fileConverter->ConvertFileToOriginal(fileInfo);
            fileInfo->compressedSize = Utils::GetFileSize(fileInfo->fullFilePath);

            //Delete the tmp huffman .txt file
            //If the original file is not .bmp, delete the tmp .bmp file
            //CleanupFiles(fileInfo);
            
            executeEnd = std::chrono::system_clock::now();
            DisplayResults();

            outFiles++;
        }
        catch(std::pair<std::exception, std::string>& e){
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    return inFiles - outFiles;
}

void ImageCompressionEngine::DisplayResults(){
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::seconds>(executeEnd-executeStart).count() << " | " 
    << fileInfo->fileName + "." + fileInfo->ext << " | " << "Initial File Size: " << fileInfo->initialSize << " | " 
    << "Compressed File Size: " << fileInfo->compressedSize << std::endl;
}

/* Private Functions */
void ImageCompressionEngine::CleanupFiles(FileConverter::FileInfo* fileInfo){
    Utils::DeleteFile(fileInfo->tmpPath);
    if(fileInfo->type != FileConverter::FileType::BMP){
        int rB = Utils::DeleteFile(fileInfo->bmpPath);
    }
}