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
        unsigned char headerBytes[54];
    
        struct PixFreq* pixFreq;
        struct HuffCode* huffCode;

        FILE* file;
    
};
#endif