#ifndef ENCODINGCONTEXT_H
#define ENCODINGCONTEXT_H

#include "./Context.h"
#include "../Artifacts/PixelFrequencies.h"
#include "../Decoding/DecodingStrategy.h"
#include "../FileHandling/FileMarshaller.h"
#include "../FileHandling/FileObjects/File.h"
#include "../ImageHandling/ImageMarshaller.h"
#include "../ImageHandling/ImageObjects/BMPImage.h"

#include <memory>
#include <vector>

class DecodingStrategy;

class EncodingContext : public Context{
    public:
        virtual void Encode(File& currentFile) = 0;
        virtual void Decode(File& currentFile) = 0;
        virtual ~EncodingContext(){
        }
    protected: 
        std::shared_ptr<DecodingStrategy> decodingStrategy;
        std::shared_ptr<BMPImage> image;        
        
        void BuildImage(){
            image = std::shared_ptr<BMPImage>(new BMPImage(contextFilePath));
            ImageMarshaller::instance().ParseImage(image); 
        }

        std::shared_ptr<BMPImage>GetBMPImage(){
            return image;
        }
        EncodingContext(){ }
        

        
};

#endif