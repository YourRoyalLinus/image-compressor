#ifndef ENCODINGCONTEXT_H
#define ENCODINGCONTEXT_H

#include "./Context.h"
#include "../Artifacts/BMPImage.h"
#include "../Artifacts/PixelFrequencies.h"
#include "../Decoding/DecodingStrategy.h"
#include "../FileHandling/FileMarshaller.h"
#include "../FileHandling/FileObjects/File.h"
#include "../ImageHandling/ImageParser.h"


class DecodingStrategy;

class EncodingContext : public Context{
    public:
        virtual void Encode(File& currentFile, FileMarshaller& marshaller) = 0;
        virtual void Decode(File& currentFile, FileMarshaller& marshaller) = 0;
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
        DecodingStrategy* decodingStrategy;  
        void BuildImage(){
            BMPImage* img = new BMPImage(contextFilePath);
            ImageParser::instance().ParseImage(*img); 
            AddArtifact(Artifact::ArtifactType::BMPIMAGE, img);
        }
        EncodingContext(){ }
        
};

#endif