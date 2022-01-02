#include "../../include/ImageCompression/BatchItem.h"

BatchItem::BatchItem(std::shared_ptr<File> f) : _file(f), originalPath(f->fullPath), startSize(f->size), isEncoded(f->isEncoded){
}

void BatchItem::ExecuteStart(){
    executeStartTime = std::chrono::system_clock::now();
}

void BatchItem::ExecuteEnd(){
     executeEndTime = std::chrono::system_clock::now();
}