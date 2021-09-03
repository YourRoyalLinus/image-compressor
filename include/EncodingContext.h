#ifndef ENCODINGCONTEXT_H
#define ENCODINGCONTEXT_H

#include "../include/File.h"

#include "Context.h"
#include "BMPImage.h"
#include "ImageParser.h"
#include "PixelFrequencies.h"
#include "FileMarshaller.h"

class EncodingContext : public Context{
    public:
        virtual void Encode(File& currentFile, FileMarshaller& marshaller) = 0;
        PixelFrequencies* GetPixelFrequencies(){
            Artifact* ptr = Context::GetArtifact(Artifact::ArtifactType::PIXELFREQUENCIES);
            PixelFrequencies* pf = dynamic_cast<PixelFrequencies*>(ptr);
            return pf;
        }
        BMPImage* GetBMPImage(){
            Artifact* ptr = Context::GetArtifact(Artifact::ArtifactType::BMPIMAGE);
            BMPImage* img = dynamic_cast<BMPImage*>(ptr);
            return img;
        }
    protected:        
        void BuildImage(){
            BMPImage* img = new BMPImage(contextFilePath);
            ImageParser::instance().ParseImage(*img); 
            AddArtifact(Artifact::ArtifactType::BMPIMAGE, img);
        }
        EncodingContext(){ }
};

#endif