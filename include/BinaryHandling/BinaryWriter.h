#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include "../Utils/Uncopyable.h"
#include <memory>
#include <fstream>

class BinaryWriter : private Uncopyable{
    public:
        BinaryWriter();
        BinaryWriter(std::shared_ptr<std::ofstream> ofStream);
        BinaryWriter(std::shared_ptr<std::ofstream> ofStream, int offSet);
        ~BinaryWriter();

        void HandleNextBit(unsigned char bit);
        void HandleNextByte(unsigned char byte);
        
        bool IsPaddingRequired() const;
        void CalculatePadding();
        unsigned short int GetPadding();
    private:
        void WriteByteToFile();
        void LeftShiftBitByOne();
        void ClearByte();
        bool IsByteFull() const;

        int _totalBits = 0;
        unsigned short int _padding = 0;
        unsigned char _currentByte;
        std::shared_ptr<std::ofstream> _ofStream;

};

#endif