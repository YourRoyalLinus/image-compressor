#include "ImageCompressionEngine.h"
#include "Utils.h"
#include <getopt.h>
#include <fstream>

int main(int argc, char **argv){
    std::string batchFilePath;
    std::vector<std::string> inputFiles;
    const std::string helpMsg = "This program can compress: BMP, JPEG, PNG, and TiFF files.\n"
                        "If leave the command line empty, the program will prompt you for a file.\n"
                        "You can input multiple files in the command line by seperating each file path with a comma (,).\n"
                        "Flags: \n"
                        "--file=BATCHFILE \t\t Text file(.txt) of files to be compressed. Seperated by newlines.\n"
                        "--help \t\t\t\t Print this message and exit.\n";
    int opt;

    int thisOptionOptInd = optind ? optind : 1;
    int optionIndex = 0;
    static struct option longOptions[] = {
        {"help",    no_argument     , 0,  'h'},
        {"file",    required_argument, 0, 'f'},
        {0,         0,                 0,  0 }
    };

    while (1) {
        std::fstream batchFile;
        opt = getopt_long_only(argc, argv, "", longOptions, &optionIndex);
        if(opt == -1) break;
        switch(opt){
            case 'h':
                std::cout << helpMsg;
                exit(0);
                break;
            case 'f':
                batchFilePath = optarg;
                batchFile = std::fstream(batchFilePath, std::ios::in);
                if(!batchFile.is_open()){
                    std::cout << "Unable to open " << batchFilePath << "\n";
                    exit(-1);
                }
                char line[256];
                while(batchFile.getline(line, 256)){
                    inputFiles.push_back(line);
                }
                break;
            default:
                std::cout << "Invalid command line argument\n";
                exit(-1);
                break;
        }
    }

    if(argc < 2){
        //Promt user for image (at least one)
        std::string input;
        std::cout << "Enter the path of valid file(s) to be compressed. Seperate multiple files by a comma (,): ";
        std::cin >> input;
        inputFiles = Utils::StringSplit(input, ','); //todo test
        for(unsigned i = 0; i < inputFiles.size(); i++){
            if(Utils::ValidFileType(inputFiles[i])){
                continue;
            }
            else{
                //Remove the current file from the inputFiles list, as it is not a valid file type
                auto start = inputFiles.begin()+i;
                auto end = start+1; 
                inputFiles.erase(start, end);
            }
        }
    }
    else{
        //Get all images from cmdargs
        for(unsigned i = 1; i < argc; i++){
            //If the file from cmd args is a valid file type, include it
            if(Utils::ValidFileType(argv[i])){
                inputFiles.push_back(argv[i]);
            }
        } 
    }

    std::string usrPath = Utils::FindUserPath();
    std::string homePath = usrPath +"/ImageCompressor";

    if(!std::experimental::filesystem::exists(homePath)){
        homePath = Utils::CreateFolder(usrPath, "ImageCompressor");
    }

    if(inputFiles.size() > 0){
        //if images, create timestamp parent folder to group images
        std::string timestamp = Utils::GetTimeStamp();
        std::string timestampPath = homePath + "/" + timestamp;
        std::string inboundPath = timestampPath + "/inbound";
        std::string outboundPath = timestampPath + "/outbound";

        Utils::CreateFolder(homePath, timestamp);

        if(std::experimental::filesystem::exists(timestampPath)){
            //create inbound and outbound children folders
            if(!std::experimental::filesystem::exists(inboundPath)){
                Utils::CreateFolder(timestampPath, "inbound");
            }

            if(!std::experimental::filesystem::exists(outboundPath)){
                Utils::CreateFolder(timestampPath, "outbound");
            }
        }

        ImageCompressionEngine* ice = new ImageCompressionEngine(inboundPath, outboundPath, timestampPath, inputFiles); 
        int res = ice->StartBatchCompression();
        if(res != 0){
            std::cout << "Batch completed succesfully, however " << res << " files were unable to be compiled" << std::endl;
            exit(0);
        }
    }
    else{
        exit(0);
    }
}