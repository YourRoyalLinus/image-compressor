#ifndef FILEMETADATA_H
#define FILEMETADATA_H

#include <vector>
#include <string>

class FileMetadata{
    public:
        FileMetadata(std::string fullFilePath);

        std::vector<std::string> pathTokens;
        std::vector<std::string> pathComponents;
    private:  
        void GetPathTokens(std::string fullFilePath);
        void GetPathComponents();
};

#endif