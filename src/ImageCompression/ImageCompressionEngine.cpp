#include "../../include/ImageCompression/ImageCompressionEngine.h"
#include "../../include/ImageCompression/Compressor.h"
#include <assert.h>

ImageCompressionEngine::ImageCompressionEngine(Batch* b) : batch(b),  fileMarshaller(new FileMarshaller()){
}

ImageCompressionEngine::ImageCompressionEngine(Batch* b, FileMarshaller* fm) : batch(b),  fileMarshaller(fm){
}

ImageCompressionEngine::~ImageCompressionEngine(){
    if(batch != nullptr){
        delete(batch);
        batch = nullptr;
    }

    if(fileMarshaller != nullptr){
        delete(fileMarshaller);
        fileMarshaller = nullptr;
    }
}


int ImageCompressionEngine::StartBatchCompression(){ //BREAKUP FURTHER
    int batchSize = batch->inputFiles.size();

    for(unsigned i = 0; i < batchSize; i++){
        try{
            batch->ExecuteStart();

            std::string f = batch->inputFiles[i];
            batch->SetActiveItem(f);
            File* currentFile = fileMarshaller->InitializeFile(f);
            if(!fileMarshaller->IsValidFileType(currentFile->ext)){
                batch->ItemSuccessfullyProcessed(false);
                std::cout << currentFile->ext << " compression is not supported" << std::endl;
            }
            fileMarshaller->ParseFile(*currentFile);
            long startSize = currentFile->size; 

            std::string inboundFilePath = fileMarshaller->CreateLocalCopy(batch->inboundPath,  *currentFile);
            std::string outboundFilePath = fileMarshaller->CreateLocalCopy(batch->outboundPath, *currentFile);
            assert(fileMarshaller->DoesPathExist(inboundFilePath) & fileMarshaller->DoesPathExist(outboundFilePath));
            fileMarshaller->UpdateFilePath(batch->outboundPath, *currentFile);

            HuffmanEncodingContext* encodingContext = new HuffmanEncodingContext(currentFile->fullPath.c_str());

            if(currentFile->isEncoded){
                encodingContext = new HuffmanEncodingContext(currentFile->fullPath.c_str());
                Compressor::DecodeImageFile(*encodingContext, *currentFile, *fileMarshaller);
                long decompressedSize = fileMarshaller->GetFileSize(currentFile->fullPath);
                 if(decompressedSize < 0){
                    batch->ItemSuccessfullyProcessed(false);
                    std::cout << "Error reading decoded file information" << std::endl;
                    continue;
                 }
                 else{
                    batch->ExecuteEnd();
                    batch->RecordExecutionResults(startSize, decompressedSize, true);
                    batch->ItemSuccessfullyProcessed(true);
                 }
            }     
            else{
                fileMarshaller->ConvertFileToBMP(*currentFile);
                
                encodingContext = new HuffmanEncodingContext(currentFile->fullPath.c_str());
                Compressor::CreateContext(*encodingContext);
                Compressor::EncodeImageFile(*encodingContext, *currentFile, *fileMarshaller);
                    
                currentFile->fullPath = currentFile->relativePath + "/" + currentFile->name + ".jcif";
                long compressedSize = fileMarshaller->GetFileSize(currentFile->fullPath);

                if(currentFile->size < 0){
                    batch->ItemSuccessfullyProcessed(false);
                    std::cout << "Error reading encoded file information" << std::endl;
                    continue;
                }
                else{
                    batch->ExecuteEnd();
                    batch->RecordExecutionResults(startSize, compressedSize, false);
                    batch->ItemSuccessfullyProcessed(true);
                }            
                
            }

            fileMarshaller->CleanUpTempFiles();
           
        }
        catch(std::pair<std::exception, std::string>& e){
            batch->ItemSuccessfullyProcessed(false);
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    return batch->CheckBatchStatus();
}