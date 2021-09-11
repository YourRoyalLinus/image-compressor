#ifndef DECODINGSTRATEGY_H
#define DECODINGSTRATEGY_H

#include "../FileHandling/FileMarshaller.h"
#include "../FileHandling/FileObjects/File.h"
#include "../FileHandling/FileObjects/FileHeader.h"



class DecodingStrategy{
    public: 
        virtual void Decode(File& currentFile, FileMarshaller& marshaller) = 0;
    protected:
        DecodingStrategy() { }   
};

#endif