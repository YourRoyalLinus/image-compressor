#ifndef IMAGECOMPRESSOR_H
#define IMAGECOMPRESSOR_H

#include "FileConverter.h"

class ImageCompressor{
    public:
        struct PixFreq{
            int pix;
            float freq;
            struct PixFreq *left, *right;
            char* code;
            
            PixFreq(){
            }

            PixFreq(int maxCodeLength){
                code = new char[maxCodeLength];
            }
        };
        struct HuffCode{
            int pix;
            int arrloc;
            float freq;
        };
        struct Header{
            std::string signature = "0100001001001101";
            int fileSize;
            int64_t reserved = 0;
            int dataOffset;
        };
        struct InfoHeader{
            int headerSize = 40;
            int height;
            int width;
            int planes = 1;
            int bitsPerPixel;
            int compression = 0;
            int imageSize = 0;
            int xPixelsPerM = 0;
            int yPixelsPerM = 0;
            int colorsUsed = 0;
            int importantColors = 0;
        };
        struct ColorTable{

        };
        ImageCompressor();
        ~ImageCompressor();

        void CompressImageFile(FileConverter::FileInfo *fileInfo);
    private:
        void ReadInto2DArray(std::string bmpPath);
        int* GetHistogram();
        int GetNonZeroOccurances(int* hist);
        int GetMaxCodeLength(float p);
        int GetTotalNodes(int nodes);
        float GetMinimumProb(int* hist);
        void InitStructs(int mcl, int nodes, int totalNodes);
        void InitArrays(int* hist);
        void SortHuffCodeArray(int nodes);
        void BuildHuffmanTree(int nodes);
        void Backtrack( int nodes, int totalNodes);
        void ConcatCodes(char* str, char* parentCode, char add);
        void EncodeCompressedImage(FileConverter::FileInfo *fileInfo, int nodes);

        int** imageArray;
        char* headerData;
        struct Header* bmpHeader;
        struct InfoHeader* bmpInfoHeader;
        
        struct PixFreq* pixFreq;
        struct HuffCode* huffCode;


        FILE* file;
    
};
#endif