#ifndef DECODINGSTRATEGY_H
#define DECODINGSTRATEGY_H

#include "FileHeader.h"
#include "FileMarshaller.h"
#include "File.h"

class DecodingStrategy{
    public: 
        virtual void Decode(File& currentFile, FileMarshaller& marshaller) = 0;
    protected:
        DecodingStrategy() { }   
};

#endif