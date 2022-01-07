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

void Batch::UpdateActiveItemSize(){
    activeItem->endSize = activeItem->_file->size;
}

void Batch::ItemExecuteStart(){
    activeItem->ExecuteStart();
}

void Batch::ItemExecuteEnd(){
    activeItem->ExecuteEnd();
}

void Batch::RecordExecutionResults(){
    const auto mins = std::chrono::duration_cast<std::chrono::minutes>(activeItem->executeEndTime - activeItem->executeStartTime);
    const auto secs = std::chrono::duration_cast<std::chrono::seconds>(activeItem->executeEndTime - activeItem->executeStartTime - mins);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(activeItem->executeEndTime - activeItem->executeStartTime - secs);

    std::string operation = (activeItem->isEncoded? "Decoded" : "Encoded");

    std::cout << "Execution Time (MIN:SEC:MS): " << mins.count() << ":" << secs.count() << ":" << ms.count() << " | "
    << activeItem->originalPath << " | " << operation << " | " "Initial File Size (Byes): " << activeItem->startSize << " | " 
    << "New File Size (Bytes): " << activeItem->endSize << " | " << std::endl;
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