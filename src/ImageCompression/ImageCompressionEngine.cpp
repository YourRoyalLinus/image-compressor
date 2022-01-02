#include "../../include/ImageCompression/ImageCompressionEngine.h"
#include "../../include/ImageCompression/Compressor.h"
#include <assert.h>

ImageCompressionEngine::ImageCompressionEngine(std::shared_ptr<Batch> b) : batch(b),  fileMarshaller(std::shared_ptr<FileMarshaller>(new FileMarshaller())){
}

ImageCompressionEngine::ImageCompressionEngine(std::shared_ptr<Batch> b, std::shared_ptr<FileMarshaller> fm) : batch(b),  fileMarshaller(fm){
}

bool ImageCompressionEngine::GetNextBatchItem(unsigned short int batchItemId){
    std::string f = batch->inputFiles[batchItemId];
    std::shared_ptr<File> currentFile = std::shared_ptr<File>(fileMarshaller->InitializeFile(f));
    fileMarshaller->ParseFile(*currentFile);
    //VALIDATE FILE
    if(currentFile->type == File::FileType::INVALID){
        batch->InvalidBatchItem();
        std::cout << currentFile->ext << " compression is not supported" << std::endl;
        return false;
    }
    else{
        batch->SetActiveItem(currentFile);
        batch->ItemExecuteStart();
        return true;
    }   
}

void ImageCompressionEngine::EncodeImage(File& currentFile){
    fileMarshaller->ConvertFileToBMP(currentFile);

    std::shared_ptr<HuffmanEncodingContext> encodingContext = Compressor::GetHuffmanEncodingContext(currentFile);
    Compressor::CreateContext(*encodingContext);
    Compressor::EncodeImageFile(*encodingContext, currentFile, *fileMarshaller);
    
    batch->UpdateActiveItemSize(currentFile);
    batch->ValidateBatchItem();   
}

void ImageCompressionEngine::DecodeImage(File& currentFile){
    std::shared_ptr<HuffmanEncodingContext> encodingContext = Compressor::GetHuffmanEncodingContext(currentFile);
    Compressor::DecodeImageFile(*encodingContext, currentFile, *fileMarshaller);

    batch->UpdateActiveItemSize(currentFile);
    batch->ValidateBatchItem();
}


void ImageCompressionEngine::CreateLocalCopies(File& currentFile){
    std::string inboundFilePath = fileMarshaller->CreateLocalCopy(batch->inboundPath,  currentFile);
    std::string outboundFilePath = fileMarshaller->CreateLocalCopy(batch->outboundPath, currentFile);
    assert(fileMarshaller->DoesPathExist(inboundFilePath) & fileMarshaller->DoesPathExist(outboundFilePath));
    fileMarshaller->UpdateFilePath(batch->outboundPath, currentFile);
}

int ImageCompressionEngine::StartBatchCompression(){ 
    int batchSize = batch->inputFiles.size();

    for(unsigned i = 0; i < batchSize; i++){
        try{
            bool next = GetNextBatchItem(i);
            if(next){
                std::shared_ptr<File> currentFile = batch->GetActiveItem();
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
            batch->InvalidBatchItem();
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    fileMarshaller->CleanUpTempFiles();
    return batch->CheckBatchStatus();
}