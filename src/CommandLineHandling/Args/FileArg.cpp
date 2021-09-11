#include "../../../include/CommandLineHandling/Args/FileArg.h"

FileArg::FileArg(std::string filePathParam){
    filePath = filePathParam;
    SetFlag();
}

void FileArg::Handle(){
    Artifact art;
    art.files.push_back(filePath);
    SetArtifact(art);
}

void FileArg::SetFlag(){
    flag = 'f';
}