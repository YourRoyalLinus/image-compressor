#include "../../include/BinaryHandling/BinaryWriter.h"

BinaryWriter::BinaryWriter(){
}

BinaryWriter::BinaryWriter(std::shared_ptr<std::ofstream> ofstream){
    _ofStream = ofstream;
}

BinaryWriter::BinaryWriter(std::shared_ptr<std::ofstream> ofstream, int fileOffset){
    ofstream->seekp(fileOffset);
    _ofStream = ofstream;
}

BinaryWriter::~BinaryWriter(){
    _ofStream->flush();
}

void BinaryWriter::HandleNextBit(unsigned char bit){
    LeftShiftBitByOne();

    if((bit ^ '1') == 0){ 
        _currentByte |= 1;
    }

    _totalBits++;

    if(IsByteFull()){
        WriteByteToFile();
        ClearByte();
    }
}

void BinaryWriter::HandleNextByte(unsigned char byte){
    _currentByte = byte;
    WriteByteToFile();
}
void BinaryWriter::WriteByteToFile(){
    _ofStream->write(reinterpret_cast<char*>(&_currentByte), sizeof(char));
}

void BinaryWriter::LeftShiftBitByOne(){
    _currentByte <<= 1;
}

void BinaryWriter::ClearByte(){
    _currentByte = 0;
}

bool BinaryWriter::IsByteFull() const{
    return (_totalBits % 8 == 0);
}

bool BinaryWriter::IsPaddingRequired() const{
    return (_totalBits % 8 != 0);
}

void BinaryWriter::CalculatePadding(){
    while(_totalBits % 8 != 0){
        LeftShiftBitByOne();
        _totalBits++;
        _padding++;
    }

    WriteByteToFile();
}

unsigned short int BinaryWriter::GetPadding(){
    return _padding;
}
