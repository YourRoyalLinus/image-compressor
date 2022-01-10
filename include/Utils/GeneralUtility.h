#ifndef GENERALUTILITY_H
#define GENERALUTILITY_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>

namespace Utility{
    inline std::string GetTimeStamp(){
        std::string timestamp;
        
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        timestamp += std::to_string(now->tm_year+1900) + "-" + std::to_string(now->tm_mon+1) + "-" + std::to_string(now->tm_mday) 
                    + "T" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);

        return timestamp;
    }

    inline std::vector<std::string> StringSplit(std::string s, char delim) {
        std::vector<std::string> tokens;
        std::size_t ix = s.find(delim);
        while (ix != std::string::npos) {
            auto val = s[0];
            if(std::isspace(val)){ //Excludes a leading space from the token
                tokens.push_back(s.substr(1, ix-1));
            }
            else{
                tokens.push_back(s.substr(0, ix));
            }            
            s.erase(0, ix + 1);
            ix = s.find(delim);
        }
        if (s.size() > 0) {
            if(std::isspace(s[0])){ //captures the text after the last comma
                tokens.push_back(s.substr(1, s.size())); //Excludes a leading space from the token  
            }
            else{
                tokens.push_back(s);
            }  
        }
        return tokens;
    }

    inline int FibonacciSeq(int n){
        if(n <= 1){
            return n;
        }
        else{
            return FibonacciSeq(n-1) + FibonacciSeq(n-2);
        }
    }
}

#endif