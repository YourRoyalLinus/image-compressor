#include "../include/ImageCompressionEngine.h"
#include "../include/Compressor.h"
#include <assert.h>

ImageCompressionEngine::ImageCompressionEngine(Batch* b) : batch(b),  fileMarshaller(new FileMarshaller()), imageCompressor(new ImageCompressor()){
}

ImageCompressionEngine::ImageCompressionEngine(Batch* b, FileMarshaller* fm) : batch(b),  fileMarshaller(fm), imageCompressor(new ImageCompressor()){
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

    if(imageCompressor != nullptr){
        delete(imageCompressor);
        imageCompressor = nullptr;
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
            //Add currentFile to cleanup
            fileMarshaller->ConvertFileToBMP(*currentFile);
            HuffmanEncodingContext* encodingContext = new HuffmanEncodingContext(currentFile->fullPath.c_str());
            Compressor::CreateContext(*encodingContext);

            //std::string encodedFilePath = currentFile->relativePath + "/" + currentFile->name + "_encoded.bin";
            //Compressor::EncodeImageFile(encodedFilePath.c_str(), *encodingContext);
            exit(0);
            imageCompressor->CompressImageFile(*currentFile);
            long compressedSize = fileMarshaller->GetFileSize(currentFile->fullPath);

            //Add currentFile to cleanup
            fileMarshaller->WriteFileToDisk(*currentFile);
            
            
            if(currentFile->size < 0){
                batch->ItemSuccessfullyProcessed(false);
                std::cout << "Error reading encoded file information" << std::endl;
            }            

            imageCompressor->DecompressImageFile(*currentFile);

            
            batch->ExecuteEnd();
            batch->RecordExecutionResults(startSize, compressedSize);
            batch->ItemSuccessfullyProcessed(true);

            //CleanupTmpFiles;
        }
        catch(std::pair<std::exception, std::string>& e){
            batch->ItemSuccessfullyProcessed(false);
            std::cout << "Unable to compress image: " << e.second << " - " <<  e.first.what() << std::endl;
        }
    }
    
    return batch->CheckBatchStatus();
}