#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H
#include "../../include/ImageHandling/ImageObjects/BaseImage.h"
#include "../../include/ImageHandling/ImageObjects/JCIFImage.h"
#include "../../lib/CImg-2.9.9/CImg.h"

class ImageMarshaller{
    public:
        static ImageMarshaller& instance();
        void ParseImage(std::shared_ptr<BaseImage> img);
        void SaveImage(File& currentFile, JCIFImage& encodedImage, cimg_library::CImg<unsigned char>& newImage);
    protected:
        ImageMarshaller();
};

#endif