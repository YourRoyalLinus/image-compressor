#ifndef ENCODINGCONTEXT_H
#define ENCODINGCONTEXT_H

#include "./Context.h"
#include "../Artifacts/PixelFrequencies.h"
#include "../Decoding/DecodingStrategy.h"
#include "../FileHandling/FileMarshaller.h"
#include "../FileHandling/FileObjects/File.h"
#include "../ImageHandling/ImageParser.h"
#include "../ImageHandling/ImageObjects/BMPImage.h"
#include <memory>

class DecodingStrategy;

class EncodingContext : public Context{
    public:
        virtual void Encode(File& currentFile, FileMarshaller& marshaller) = 0;
        virtual void Decode(File& currentFile, FileMarshaller& marshaller) = 0;
        std::shared_ptr<PixelFrequencies> GetPixelFrequencies(){
            return pf;
        }
        std::shared_ptr<BMPImage>GetBMPImage(){
            return image;
        }
        virtual ~EncodingContext(){
        }
    protected: 
        std::unique_ptr<DecodingStrategy> decodingStrategy;
        std::shared_ptr<PixelFrequencies> pf;
        std::shared_ptr<BMPImage> image;

        void BuildImage(){
            image = std::shared_ptr<BMPImage>(new BMPImage(contextFilePath));
            ImageParser::instance().ParseImage(*image.get()); 
        }
        EncodingContext(){ }
        

        
};

#endif