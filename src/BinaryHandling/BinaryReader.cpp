#include "../../include/BinaryHandling/BinaryReader.h"

BinaryReader::BinaryReader(){
}

BinaryReader::BinaryReader(std::shared_ptr<std::ifstream> ifstream, int bufSizeinBytes, unsigned short int bitPadding){
    _ifStream = ifstream;
    _padding = bitPadding;
    InitializeBuffer(bufSizeinBytes);
}


bool BinaryReader::AllBitsRead() const{
    return (_currentBits == (_bufSizeBits - _padding));
}

int BinaryReader::GetNextCode(){
    HandleNextBit();
    return (_currentByte >> _msb) & 1;
}

void BinaryReader::HandleNextBit(){
    if(IsByteExhausted()){
        ReadByteFromBuf();
        ResetBitIndex();
    }
    
    ShiftMostSignificantBit();
    _currentBits++;
}

void BinaryReader::InitializeBuffer(int bufferSizeInBytes){
    buf = new unsigned char[bufferSizeInBytes];
    _bufSizeBytes = bufferSizeInBytes;
    _bufSizeBits = (bufferSizeInBytes*8);

    ReadBytesIntoBuf();
    ReadByteFromBuf();
}

void BinaryReader::ReadBytesIntoBuf(){
    _ifStream->read(reinterpret_cast<char*>(buf), _bufSizeBytes);
}

void BinaryReader::ReadByteFromBuf(){
    _currentByte = buf[_byteIndex++];
}

void BinaryReader::ResetBitIndex(){
    _bitIndex = 0;
}

void BinaryReader::ShiftMostSignificantBit(){
    _msb = 7 - _bitIndex++;
}

bool BinaryReader::IsByteExhausted() const{
    return (_bitIndex == 8);
}