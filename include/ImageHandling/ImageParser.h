#ifndef IMAGEPARSER_H
#define IMAGEPARSER_H

#include "../Artifacts/BMPImage.h"

class ImageParser{ //TODO Base Parser Class/Small refactor
    public:
        static ImageParser& instance();
        void ParseImage(BMPImage& img);
    protected:
        ImageParser();
    private:
        void FetchFileHeaderData(BMPImage& img);
        void CreateCImage(BMPImage& img);
        void FetchPixelDataArray(BMPImage& img);
};

#endif