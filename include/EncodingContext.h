#ifndef ENCODINGCONTEXT_H
#define ENCODINGCONTEXT_H

#include "Context.h"
#include "BMPImage.h"
#include "ImageParser.h"
#include "PixelFrequencies.h"


class EncodingContext : public Context{
    public:
        PixelFrequencies* GetPixelFrequencies(){
            Artifact* ptr = Context::GetArtifact(Context::BuildArtifact::PIXELFREQUENCIES);
            PixelFrequencies* pf = dynamic_cast<PixelFrequencies*>(ptr);
            return pf;
        }
        BMPImage* GetBMPImage(){
            Artifact* ptr = Context::GetArtifact(Context::BuildArtifact::BMPIMAGE);
            BMPImage* img = dynamic_cast<BMPImage*>(ptr);
            return img;
        }
    protected:        
        void BuildImage(){
            BMPImage* img = new BMPImage(contextFilePath);
            ImageParser::instance().ParseImage(*img); 
            AddArtifact(Context::BuildArtifact::BMPIMAGE, img);
        }
        EncodingContext(){ }
};

#endif