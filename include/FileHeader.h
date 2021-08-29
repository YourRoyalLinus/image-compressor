#ifndef FILEHEADER_H
#define FILEHEADER_H

class FileHeader{
    public:
        FileHeader(unsigned char* buffer){
            _buffer = buffer;
            GetFileHeaderData();
            GetInfoHeaderData();
        }

        ~FileHeader(){
            FreeBufferPtr();
        }

        unsigned char* WriteToBuffer(){
            FreeBufferPtr();
            WriteFileHeaderData();
            WriteFileInfoData();
            return _buffer;
        }

        const unsigned int size = 54;

        unsigned char fileType = 0;
        unsigned int fileSize = 0;
        unsigned short int reservedByteOne = 0;
        unsigned short int reservedByteTwo = 0;
        unsigned int pixelDataOffset = 0;

        unsigned int headerSize = 0;
        int imageWidth = 0;
        int imageHeight = 0;
        unsigned short int planes = 0;
        unsigned short int bitsPerPixel = 0;
        unsigned int compression = 0;
        unsigned int imageSize = 0;

        int xPixelsPerMeter = 0;
        int yPixelsPerMeter = 0;
        unsigned int totalColors = 0;
        unsigned int importantColors = 0;
        
    private:
        unsigned char* _buffer;

        void FreeBufferPtr(){
            delete[] _buffer;
        }

        void GetFileHeaderData(){
            fileType = GetFileType();
            fileSize = GetFileSize();
            reservedByteOne = GetReservedByteOne();
            reservedByteTwo = GetReservedByteTwo();
            pixelDataOffset = GetPixelDataOffset();
        }

        void GetInfoHeaderData(){
            headerSize = GetHeaderSize();
            imageWidth = GetImageWidth();
            imageHeight = GetImageHeight();
            planes = GetPlanes();
            bitsPerPixel = GetBitsPerPixel();
            compression = GetCompression();
            imageSize = GetImageSize();
            xPixelsPerMeter = GetXPixelsPerMeter();
            yPixelsPerMeter = GetYPixelsPerMeter();
            totalColors = GetTotalColors();
            importantColors = GetImportantColors();
        }

        unsigned char GetFileType(){
            return Read2UnsignedBytesStartingAt(fileTypeStartByte);
        }

        unsigned int GetFileSize(){
            return Read4UnsignedBytesStartingAt(fileSizeStartByte);
        }

        unsigned short int GetReservedByteOne(){
            return Read2UnsignedBytesStartingAt(reservedOneStartByte);
        }

        unsigned short int GetReservedByteTwo(){
            return Read2UnsignedBytesStartingAt(reservedTwoStartByte);
        }

        unsigned int GetPixelDataOffset(){
            return Read4UnsignedBytesStartingAt(pixelDataOffsetStartByte);
        }

        unsigned int GetHeaderSize(){
            return Read4UnsignedBytesStartingAt(headerSizeStartByte);
        }

        int GetImageWidth(){
            return Read4SignedBytesStartingAt(imageWidthStartByte);
        }

        int GetImageHeight(){
            return Read4SignedBytesStartingAt(imageHeightStartByte);
        }

        unsigned short int GetPlanes(){
            return Read2UnsignedBytesStartingAt(planesStartByte);
        }

        unsigned short int GetBitsPerPixel(){
            return Read2UnsignedBytesStartingAt(bitsPerPixelStartByte);
        }

        unsigned int GetCompression(){
            return Read4UnsignedBytesStartingAt(compressionStartByte);
        }

        unsigned int GetImageSize(){
            return Read4UnsignedBytesStartingAt(imageSizeStartByte);
        }

        int GetXPixelsPerMeter(){
            return Read4SignedBytesStartingAt(xPixelsPerMeterStartByte);
        }

        int GetYPixelsPerMeter(){
            return Read4SignedBytesStartingAt(yPixelsPerMeterStartByte);
        }

        unsigned int GetTotalColors(){
            return Read4UnsignedBytesStartingAt(totalColorsStartByte);
        }

        unsigned int GetImportantColors(){
            return Read4UnsignedBytesStartingAt(importantColorsStartByte);
        }

        unsigned short int Read2UnsignedBytesStartingAt(int ix){
            return *(unsigned short int*)&_buffer[ix];
        }

        unsigned int Read4UnsignedBytesStartingAt(int ix){
            return *(unsigned int*)&_buffer[ix];
        }

        int Read4SignedBytesStartingAt(int ix){
            return *(int*)&_buffer[ix];
        }

        void WriteFileHeaderData(){
            WriteBytesToBuffer(fileTypeStartByte, fileType);
            WriteBytesToBuffer(fileSizeStartByte, fileSize);
            WriteBytesToBuffer(reservedOneStartByte, reservedByteOne);
            WriteBytesToBuffer(reservedTwoStartByte, reservedByteTwo);
            WriteBytesToBuffer(pixelDataOffsetStartByte, pixelDataOffset);
        }

        void WriteFileInfoData(){
            WriteBytesToBuffer(headerSizeStartByte, headerSize);
            WriteBytesToBuffer(imageWidthStartByte, imageWidth);
            WriteBytesToBuffer(imageHeightStartByte, imageHeight);
            WriteBytesToBuffer(planesStartByte, planes);
            WriteBytesToBuffer(bitsPerPixelStartByte, bitsPerPixel);
            WriteBytesToBuffer(compressionStartByte, compression);
            WriteBytesToBuffer(imageSizeStartByte, imageSize);
            WriteBytesToBuffer(xPixelsPerMeterStartByte, xPixelsPerMeter);
            WriteBytesToBuffer(yPixelsPerMeterStartByte, yPixelsPerMeter);
            WriteBytesToBuffer(totalColorsStartByte, totalColors);
            WriteBytesToBuffer(importantColorsStartByte, importantColors);
        }

        void WriteBytesToBuffer(unsigned int startByte, unsigned char value){
            _buffer[startByte] = value;
        }

        void WriteBytesToBuffer(unsigned int startByte, unsigned short int value){
            int byteOffset = 1;
            int shiftBits = byteOffset * 8;

            unsigned short int currentByte = startByte+byteOffset;
            while(shiftBits > 0){
                _buffer[currentByte] = (value >> shiftBits) & 0xFF;
                shiftBits -= 8;
                currentByte--;   
            }
            _buffer[currentByte] = shiftBits & 0xFF;
            
        }

        void WriteBytesToBuffer(unsigned int startByte, unsigned int value){
            int byteOffset = 3;
            unsigned int shiftBits = byteOffset * 8;

            unsigned short int currentByte = startByte+byteOffset;
            while(shiftBits > 0){
                _buffer[currentByte] = (value >> shiftBits) & 0xFF;
                shiftBits -= 8;
                currentByte--;   
            }
            _buffer[currentByte] = shiftBits & 0xFF;
        }

        void WriteBytesToBuffer(unsigned int startByte, int value){
            int byteOffset = 3;
            int shiftBits = byteOffset * 8;

            unsigned short int currentByte = startByte+byteOffset;
            while(shiftBits > 0){
                _buffer[currentByte] = (value >> shiftBits) & 0xFF;
                shiftBits -= 8;
                currentByte--;   
            }
            _buffer[currentByte] = shiftBits & 0xFF;
        }

        const unsigned char fileTypeStartByte = 0;
        const unsigned char fileSizeStartByte = 2;
        const unsigned char reservedOneStartByte = 6;
        const unsigned char reservedTwoStartByte = 8;
        const unsigned char pixelDataOffsetStartByte = 10;

        const unsigned char headerSizeStartByte = 14;
        const unsigned char imageWidthStartByte = 18;
        const unsigned char imageHeightStartByte = 22;
        const unsigned char planesStartByte = 26;
        const unsigned char bitsPerPixelStartByte = 28;
        const unsigned char compressionStartByte = 30;
        const unsigned char imageSizeStartByte = 34;
        const unsigned char xPixelsPerMeterStartByte = 48;
        const unsigned char yPixelsPerMeterStartByte = 42;
        const unsigned char totalColorsStartByte = 46;
        const unsigned char importantColorsStartByte = 50;   
};

#endif
