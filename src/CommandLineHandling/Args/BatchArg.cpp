#include "../../../include/CommandLineHandling/Args/BatchArg.h"
#include "../../../include/FileHandling/FileMarshaller.h"
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
    if(!FileMarshaller::instance().IsValidBatchFile(batchFilePath)){
        std::cout << "Invalid Batch File. Only newline seperated \".txt\" batch files are supported" << std::endl;
        exit(-1);
    }
    else if(!batchFile.is_open()){
        std::cout << "Unable to open batch file: " << batchFilePath << std::endl;
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
