#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include "./FileSystem.h"
#include "./FileObjects/File.h"
#include "../../lib/CImg-2.9.9/CImg.h"

//Requires ImageMagik

class FileConverter{ 
    public:
        static FileConverter& instance();
        void ConvertFileToBMP(File& file);
        void ConvertFileToTIFF(File& file);
        void ConvertImageToBMP(File& file, cimg_library::CImg<unsigned char> img);
        void ConvertImageToTIFF(File& file, cimg_library::CImg<unsigned char> img);
    protected:
        FileConverter();
    private:
        inline static std::string imageMagickPath = "/usr/local/bin/convert";
};

#endif