#include "../../include/ImageCompression/Batch.h"
#include <iostream>

 Batch::Batch(std::vector<std::string> files, std::string iPath, std::string oPath) : inputFiles(files), inboundPath(iPath), outboundPath(oPath), batchSize(files.size()), successfulExecutions(0){

}

void Batch::SetActiveItem(std::string item){
    activeItem = item;
}

void Batch::ExecuteStart(){
    executeStart = std::chrono::system_clock::now();
}

void Batch::ExecuteEnd(){
     executeEnd = std::chrono::system_clock::now();
}

void Batch::RecordExecutionResults(long startSize, long compressedSize, bool wasDecoded){
    std::string operation = (wasDecoded == true ? "Decoded" : "Encoded");

    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::minutes>(executeEnd-executeStart).count() << ":" 
                                    << std::chrono::duration_cast<std::chrono::seconds>(executeEnd-executeStart).count() << ":"
                                    << std::chrono::duration_cast<std::chrono::milliseconds>(executeEnd-executeStart).count() 
                                    << "s | " 
    << activeItem << " | " << operation << " | " "Initial File Size: " << startSize << " B | " 
    << "New File Size: " << compressedSize<< " B | " << std::endl;
}

void Batch::ItemSuccessfullyProcessed(bool status){
    if(status){
        successfulExecutions++;
    }
}

int Batch::CheckBatchStatus(){
    return batchSize - successfulExecutions;
}