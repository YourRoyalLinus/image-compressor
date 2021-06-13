#include "ImageCompressionEngine.h"
#include "Utils.h"
#include <chrono>

ImageCompressionEngine::ImageCompressionEngine() : fileConverter(nullptr), imageCompressor(nullptr){
}

ImageCompressionEngine::ImageCompressionEngine(std::string iDir, std::string oDir, std::string tsDir, std::vector<std::string> ifiles) : inboundDir(iDir),
outboundDir(oDir), timestampDir(tsDir), inputFiles(ifiles), fileConverter(nullptr), imageCompressor(nullptr){
}

ImageCompressionEngine::~ImageCompressionEngine(){
    CleanupResources();
}

std::string ImageCompressionEngine::CreateLocalInboundCopy(){
    std::string destPath = inboundDir + "/" + fileInfo->fileName + "." + fileInfo->ext;
    std::experimental::filesystem::copy(fileInfo->fullFilePath, destPath);
    currentFile = fopen(destPath.c_str(), "r");
    if(currentFile == nullptr){ 
        return "";
    }
    else{
        fclose(currentFile);
        return destPath;
    }
}

std::string ImageCompressionEngine::CreateLocalOutboundCopy(){
    std::string destPath = outboundDir + "/" + fileInfo->fileName + "." + fileInfo->ext;
    std::experimental::filesystem::copy(fileInfo->fullFilePath, destPath);
    currentFile = fopen(destPath.c_str(), "r");
    if(currentFile == nullptr){
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
            AllocResources();
            std::string f = inputFiles[i];

            fileInfo = fileConverter->ParseFile(f);
            std::string inboundFile = CreateLocalInboundCopy();
            std::string outboundFile = CreateLocalOutboundCopy();

            if(inboundFile.length() == 0 || outboundFile.length() == 0){
                std::cout << "Error opening file: " << fileInfo->fileName + "." + fileInfo->ext << std::endl;
                continue;
            }

            fileInfo = fileConverter->ParseFile(outboundFile);
            fileInfo->tmpPath = outboundFile;

            fileConverter->ConvertFileToBMP(fileInfo);
            
            //Compress image
            imageCompressor->CompressImageFile(fileInfo);

            fileInfo->encodedSize = fileConverter->SaveEncodedDataFile(fileInfo);
            if(fileInfo->encodedSize < 0){
                std::cout << "Error reading encoded file information" << std::endl;
                exit(-1);
            }            

            CleanupFiles(fileInfo);

            executeEnd = std::chrono::system_clock::now();
            DisplayResults();
            
            CleanupResources();
            outFiles++;
        }
        catch(std::pair<std::exception, std::string>& e){
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    return inFiles - outFiles;
}

void ImageCompressionEngine::DisplayResults(){
    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::minutes>(executeEnd-executeStart).count() << ":" 
                                    << std::chrono::duration_cast<std::chrono::seconds>(executeEnd-executeStart).count() << ":"
                                    << std::chrono::duration_cast<std::chrono::milliseconds>(executeEnd-executeStart).count() 
                                    << "s| " 
    << fileInfo->fileName + "." + fileInfo->ext << " | " << "Initial File Size: " << fileInfo->initialSize << " B | " 
    << "Encoded Image Size: " << fileInfo->encodedSize << " B | " << std::endl;
}

/* Private Functions */
void ImageCompressionEngine::CleanupFiles(FileConverter::FileInfo* fileInfo){
    int rb = Utils::DeleteFile(fileInfo->tmpPath);
    if(fileInfo->type != FileConverter::FileType::BMP){
        rb & Utils::DeleteFile(fileInfo->bmpPath);
    }
    if(!rb){
        std::cout << "Error deleting the tmp & bmp files." << std::endl;
    }
}

void ImageCompressionEngine::AllocResources(){
    imageCompressor = new ImageCompressor();
    fileConverter = new FileConverter();
}

void ImageCompressionEngine::CleanupResources(){
    if(imageCompressor != nullptr){
        delete imageCompressor;
        imageCompressor = nullptr;
    }

    if(fileConverter != nullptr){
        delete fileConverter;
        fileConverter = nullptr;
    }
}