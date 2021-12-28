#ifndef IMAGEPARSER_H
#define IMAGEPARSER_H

#include "./ImageObjects/BMPImage.h"
#include "./ImageObjects/JCIFImage.h"

class ImageParser{
    public:
        static ImageParser& instance();
        void ParseImage(BMPImage& img);
        void ParseImage(JCIFImage& img);
    protected:
        ImageParser();
    private:
        static const unsigned int headerSize = 54;
        void FetchFileHeaderData(BaseImage& img);
        void CreateCImage(BMPImage& img);
        void FetchPixelDataArray(BMPImage& img);
};

#endif