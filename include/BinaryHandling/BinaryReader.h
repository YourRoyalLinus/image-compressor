#ifndef BINARYREADER_H
#define BINARYREADER_H

#include "../Utils/Uncopyable.h"
#include <memory>
#include <fstream>

class BinaryReader : private Uncopyable{
    public:
        BinaryReader();
        BinaryReader(std::shared_ptr<std::ifstream> ifstream, int bufSizeinBytes, unsigned short int bitPadding);
    
        int GetNextCode();
        bool AllBitsRead() const;
    private:
        void HandleNextBit();
        void InitializeBuffer(int bufferSizeInBytes);
        void ReadBytesIntoBuf();
        void ReadByteFromBuf();
        void ResetBitIndex();
        void ShiftMostSignificantBit();
        bool IsByteExhausted() const;

        int _currentBits = 0;
        int _byteIndex = 0;
        int _bitIndex = 0;
        int _msb = 0;
        
        unsigned char _currentByte;
        
        int _bufSizeBytes;
        int _bufSizeBits;
        unsigned short int _padding;
        unsigned char* buf;
        std::shared_ptr<std::ifstream> _ifStream;

};

#endif