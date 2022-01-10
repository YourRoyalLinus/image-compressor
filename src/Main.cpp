#include "../include/CommandLineHandling/CommandLineHandler.h"
#include "../include/FileHandling/FileMarshaller.h"
#include "../include/ImageCompression/ImageCompressionEngine.h"
#include "../include/ImageCompression/Batch.h"
#include "../include/Utils/GeneralUtility.h"

#include <memory>
#include <assert.h>
#include <fstream>

int main(int argc, char **argv){
    CommandLineHandler cmdHandler = CommandLineHandler();

    //Handles first argument
    //Current arguments don't support multiple args
    cmdHandler.ParseArgs(argc, argv); 
    cmdHandler.ProcessArg();
    std::vector<std::string> inputFiles = cmdHandler.RetrieveArtifact().files;

    for(unsigned i = 0; i < inputFiles.size(); i++){
        if(FileMarshaller::instance().DoesPathExist(inputFiles[i])){
            continue;
        }
        else{
            std::cout << inputFiles[i] << " is an invald file" <<  std::endl;
            auto start = inputFiles.begin()+i;
            auto end = start+1; 
            inputFiles.erase(start, end);
        }
    }

    std::string homePath = FileMarshaller::instance().CreateHomePath();
    if(inputFiles.size() > 0){
        std::string timestamp = Utility::GetTimeStamp();
        std::string timestampPath = FileMarshaller::instance().CreatePath(homePath, timestamp);
        assert(FileMarshaller::instance().DoesPathExist(timestampPath));

        std::string inboundPath = timestampPath + "/inbound";
        if(!FileMarshaller::instance().DoesPathExist(inboundPath)){
                FileMarshaller::instance().CreatePath(timestampPath, "inbound");
        }

        std::string outboundPath = timestampPath + "/outbound";
        if(!FileMarshaller::instance().DoesPathExist(outboundPath)){
            FileMarshaller::instance().CreatePath(timestampPath, "outbound");
        }

        Batch batch = Batch(inputFiles, inboundPath, outboundPath);
        ImageCompressionEngine ice(batch); 
        int res = ice.StartBatchCompression();
        if(res != 0){
            std::cout << "Batch completed succesfully, however " << res << " files were unable to be compressed" << std::endl;
            exit(0);
        }
    }
    else{
        std::cout << "There were no valid files to be compresssed." << std::endl << "Exiting..." << std::endl;
        exit(0);
    }
}