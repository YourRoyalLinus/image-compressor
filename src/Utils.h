#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <experimental/filesystem>


class Utils{
    public:
        /*
            Creates a new folder (destFolder) within the srcPath directory
        */
        static std::string CreateFolder(std::string srcPath, std::string destFolder) {
            std::string homePath = srcPath + "/" + destFolder;
            if (std::experimental::filesystem::exists(homePath)) return homePath;;
            std::experimental::filesystem::create_directory(homePath);
            if (!FileExists(homePath)) {
                std::cout << "Error while creating Database " << destFolder << " folder";
                exit(1);
            }
            return homePath;
        }

        /* Returns the size of the File or -1 if unable to get the size */

        static long GetFileSize(std::string file){
            struct stat stat_buf;
            int rc = stat(file.c_str(), &stat_buf);
            return rc == 0 ? stat_buf.st_size : -1;
        }
        /*
            Returns true if file was deleted or false if it didn't exist. If false, prints the error message
        */
        static bool DeleteFile(const std::string& filePath){
            std::error_code ec;

            bool r = std::experimental::filesystem::remove(filePath, ec);
            if(!r){
                std:: cout << "Error deleting file: " << filePath << " - " << ec.message() << std::endl;
            }

            return r;
        }

        /*
            Returns the path to /usr on Linux
        */
        static std::string FindUserPath() {
            struct passwd *pw = getpwuid(getuid());
            const char *homedir = pw->pw_dir;
            return std::string(homedir);
        }

        /*
            Returns true if file exists, false otherwise
        */
        static bool FileExists(std::string file) {
            if (std::experimental::filesystem::exists(file)) {
                return true;
            }
            else {
                return false;
            }
        }

        /*
            Returns the current Timestamp as a string in the format YYYY-MM-DDTHH:MM:SS
        */
        static std::string GetTimeStamp(){
            std::string timestamp;
            
            std::time_t t = std::time(0);
            std::tm* now = std::localtime(&t);
            timestamp += std::to_string(now->tm_year+1900) + "-" + std::to_string(now->tm_mon+1) + "-" + std::to_string(now->tm_mday) 
                        + "T" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);

            return timestamp;
        }

        /*
            Function that parses a full or relative file path and returns true if the file type is valid/is one of: .bmp, .jpeg, .png, .tif, or false otherwise
        */
        static bool ValidFileType(std::string filePath){
            std::string validFiles[6] = { "bmp", "jpg", "jpeg", "png", "tif", "tiff" };
            std::vector<std::string> tokens = StringSplit(filePath, '.');
            std::string ext = tokens[tokens.size()-1];
            if(find(validFiles, validFiles+5, ext) == validFiles+6){
                std::cout << "\"" << ext << "\" is not one of the supported file types\n";
                return false;
            }

            return true;
        }
        
        /*
            Function that split's a string by "," and returns a vector of string tokens seperated by delim character
        */
        static std::vector<std::string> StringSplit(std::string s, char delim) {
            std::vector<std::string> tokens;
            std::size_t ix = s.find(delim);
            while (ix != std::string::npos) {
                auto val = s.substr(0, ix);
                tokens.push_back(s.substr(0, ix));
                s.erase(0, ix + 1);
                ix = s.find(delim);
            }
            if (s.size() > 0) {
                tokens.push_back(s); //captures the text after the last comma
            }
            return tokens;
        }

        /*
            Fibonacci sequence
        */
       static int Fib(int n){
           if(n <= 1){
               return n;
           }
           else{
               return Fib(n-1) + Fib(n-2);
           }
       }

};

#endif