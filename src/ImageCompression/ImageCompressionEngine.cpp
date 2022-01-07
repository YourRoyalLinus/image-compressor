#include "../../include/FileHandling/FileMarshaller.h"
#include "../../include/ImageCompression/ImageCompressionEngine.h"
#include "../../include/ImageCompression/Compressor.h"
#include <assert.h>

ImageCompressionEngine::ImageCompressionEngine(Batch& b) : batch(b){
}

bool ImageCompressionEngine::GetNextBatchItem(unsigned short int batchItemId){
    std::string f = batch.inputFiles[batchItemId];
    std::shared_ptr<File> currentFile = std::shared_ptr<File>(FileMarshaller::instance().InitializeFile(f));
    FileMarshaller::instance().ParseFile(*currentFile);

    if(!Compressor::ValidateFile(*currentFile)){
        batch.InvalidBatchItem();
        return false;
    }
    else{
        batch.SetActiveItem(currentFile);
        batch.ItemExecuteStart();
        return true;
    }   
}

void ImageCompressionEngine::EncodeImage(File& currentFile){
    FileMarshaller::instance().ConvertFileToBMP(currentFile);

    std::unique_ptr<HuffmanEncodingContext> encodingContext = Compressor::GetHuffmanEncodingContext(currentFile);
    Compressor::CreateContext(*encodingContext);
    Compressor::EncodeImageFile(*encodingContext, currentFile);
    
    batch.UpdateActiveItemSize();
    batch.ValidateBatchItem();   
}

void ImageCompressionEngine::DecodeImage(File& currentFile){
    std::unique_ptr<HuffmanEncodingContext> encodingContext = Compressor::GetHuffmanEncodingContext(currentFile);
    Compressor::DecodeImageFile(*encodingContext, currentFile);
    batch.UpdateActiveItemSize();
    batch.ValidateBatchItem();
}


void ImageCompressionEngine::CreateLocalCopies(File& currentFile){
    std::string inboundFilePath = FileMarshaller::instance().CreateLocalCopy(batch.inboundPath,  currentFile);
    std::string outboundFilePath = FileMarshaller::instance().CreateLocalCopy(batch.outboundPath, currentFile);
    assert(FileMarshaller::instance().DoesPathExist(inboundFilePath) && FileMarshaller::instance().DoesPathExist(outboundFilePath));
    FileMarshaller::instance().UpdateFilePath(batch.outboundPath, currentFile);
}

int ImageCompressionEngine::StartBatchCompression(){ 
    int batchSize = batch.inputFiles.size();

    for(int i = 0; i < batchSize; i++){
        try{
            bool next = GetNextBatchItem(i);
            if(next){
                std::shared_ptr<File> currentFile = batch.GetActiveItem();
                CreateLocalCopies(*currentFile);

                if(currentFile->isEncoded){
                    DecodeImage(*currentFile);
                }   
                else{
                    EncodeImage(*currentFile);   
                }           
            }
        }
        catch(std::pair<std::exception, std::string>& e){
            batch.InvalidBatchItem();
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    FileMarshaller::instance().CleanUpTempFiles();
    return batch.CheckBatchStatus();
}