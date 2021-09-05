#ifndef IMAGECOMPRESSOR_H
#define IMAGECOMPRESSOR_H

#include "../include/FileConverter.h"
#include "../include/File.h"
#include "../include/HuffmanTable.h"
#include "CImg/CImg-2.9.8_pre051821/CImg.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include <unordered_map>
#include <sstream>

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
        struct HuffTable{
            std::unordered_map<std::string, int> table;
            std::vector<unsigned short int> codeLengths;
            std::vector<std::string> codes;
            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( CEREAL_NVP(table), CEREAL_NVP(codeLengths), CEREAL_NVP(codes) ); // serialize things by passing them to the archive
            }

            HuffTable(){

            }

            HuffTable(const HuffTable &ht){
                table = ht.table;
                codeLengths = ht.codeLengths;
                codes = ht.codes;
            }


        };
        
        ImageCompressor();
        ~ImageCompressor();

        void CompressImageFile(File& file);
        void DecompressImageFile(File& file);
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
        void EncodeCompressedImage(File& file, int nodes);
        int EncodeBits(char* bits);
        void TestEncoding(std::string s);
        void TestDecoding(unsigned char * pixelArr, int width, int height);
        unsigned short int GetCodeLength(FILE* encodedFile);

        void CreateHuffTable(int nodes);
        int SerializeHuffTable(std::string encodedFilePath, int offset);
        void DeserializeHuffTable(std::string encodedFilePath, int offset, int end);
        char* DecodeBits(FILE* encodedFile, int significantBits);
        
        cimg_library::CImg<unsigned char> cimage;
        unsigned char* pixelDataArray;

        unsigned char headerBytes[54];
        struct PixFreq* pixFreq;
        struct HuffCode* huffCode;
        struct HuffTable* huffTable;
        struct HuffTable newHuffTable;
        HuffmanTable ht;
        FILE* file;   
};
#endif