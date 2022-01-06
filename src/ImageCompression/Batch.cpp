#include "../../include/ImageCompression/Batch.h"
#include <iostream>

 Batch::Batch(std::vector<std::string>& files, std::string iPath, std::string oPath) : inputFiles(files), inboundPath(iPath), outboundPath(oPath), batchSize(files.size()), successfulExecutions(0){

}

std::shared_ptr<File> Batch::GetActiveItem(){
    return activeItem->_file;
}

void Batch::SetActiveItem(std::shared_ptr<File> file){
    if(activeItem == 0){
        activeItem = std::shared_ptr<BatchItem>(new BatchItem(file));
    }
    else{
        activeItem.reset(new BatchItem(file));
    }
    
}

void Batch::UpdateActiveItemSize(File& file){
    activeItem->endSize = file.size;
}

void Batch::ItemExecuteStart(){
    activeItem->ExecuteStart();
}

void Batch::ItemExecuteEnd(){
    activeItem->ExecuteEnd();
}

void Batch::RecordExecutionResults(){
    std::string operation = (activeItem->isEncoded? "Decoded" : "Encoded");

    std::cout << "Execution Time: " << std::chrono::duration_cast<std::chrono::minutes>(activeItem->executeEndTime - activeItem->executeStartTime).count() << ":" 
                                    << std::chrono::duration_cast<std::chrono::seconds>(activeItem->executeEndTime - activeItem->executeStartTime).count() << ":"
                                    << std::chrono::duration_cast<std::chrono::milliseconds>(activeItem->executeEndTime - activeItem->executeStartTime).count() 
                                    << "s | " //TODO CHANGE LAYOUT TO BETTER REFLECT 0M:0S:0MS
    << activeItem->originalPath << " | " << operation << " | " "Initial File Size: " << activeItem->startSize << " B | " 
    << "New File Size: " << activeItem->endSize << " B | " << std::endl;
}

void Batch::InvalidBatchItem(){
    ItemSuccessfullyProcessed(false);
}

void Batch::ValidateBatchItem(){
    if(activeItem->endSize <= 0){
        ItemSuccessfullyProcessed(false);
        std::cout << "Error reading processed file information" << std::endl;
    }
    else{
        ItemExecuteEnd();
        RecordExecutionResults();
        ItemSuccessfullyProcessed(true);
    }
}
void Batch::ItemSuccessfullyProcessed(bool status){
    if(status){
        successfulExecutions++;
    }
}

int Batch::CheckBatchStatus(){
    return batchSize - successfulExecutions;
}