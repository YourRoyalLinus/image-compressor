#include "../include/BatchArg.h"
#include <iostream>

BatchArg::BatchArg(std::string filePathParam){
    batchFilePath = filePathParam;
    SetFlag();
}

void BatchArg::Handle(){
    ReadInBatchFile();
}

void BatchArg::SetFlag(){
    flag = 'b';
}

void BatchArg::ReadInBatchFile(){
    std::fstream batchFile = std::fstream(batchFilePath, std::ios::in);
    if(!batchFile.is_open()){
        std::cout << "Unable to open batch file: " << batchFilePath << "\n";
        exit(-1);
    }
    CollectFilePathsFromBatchFile(batchFile);

}

void BatchArg::CollectFilePathsFromBatchFile(std::fstream& batchFile){
    Artifact art;
    
    char line[256];
    while(batchFile.getline(line, 256)){
        art.files.push_back(line);
    }

    SetArtifact(art);

    batchFile.close();
}
