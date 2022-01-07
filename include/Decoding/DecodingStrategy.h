#ifndef DECODINGSTRATEGY_H
#define DECODINGSTRATEGY_H

#include "../FileHandling/FileObjects/File.h"

class DecodingStrategy{
    public: 
        virtual void Decode(File& currentFile) = 0;
    protected:
        DecodingStrategy() { }   
};

#endif