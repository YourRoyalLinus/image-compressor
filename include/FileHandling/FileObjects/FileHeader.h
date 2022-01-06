#ifndef FILEHEADER_H
#define FILEHEADER_H

struct FileHeader{
    public:
        FileHeader(unsigned char* buffer){
            _buffer = buffer;
            GetFileHeaderData();
            GetInfoHeaderData();
        }
        
        unsigned char* WriteToBuffer(){
            ClearBuffer(); 
            WriteFileHeaderData();
            WriteFileInfoData();
            return _buffer;
        }

        ~FileHeader(){
            delete[] _buffer; //todo fix
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
        unsigned char* _buffer = nullptr;

        void ClearBuffer(){
            _buffer = new unsigned char[size];
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
            _buffer[currentByte] = value & 0xFF;
            
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
            _buffer[currentByte] = value & 0xFF;
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
            _buffer[currentByte] = value & 0xFF;
        }

        const unsigned int fileTypeStartByte = 0;
        const unsigned int fileSizeStartByte = 2;
        const unsigned int reservedOneStartByte = 6;
        const unsigned int reservedTwoStartByte = 8;
        const unsigned int pixelDataOffsetStartByte = 10;

        const unsigned int headerSizeStartByte = 14;
        const unsigned int imageWidthStartByte = 18;
        const unsigned int imageHeightStartByte = 22;
        const unsigned int planesStartByte = 26;
        const unsigned int bitsPerPixelStartByte = 28;
        const unsigned int compressionStartByte = 30;
        const unsigned int imageSizeStartByte = 34;
        const unsigned int xPixelsPerMeterStartByte = 38;
        const unsigned int yPixelsPerMeterStartByte = 42;
        const unsigned int totalColorsStartByte = 46;
        const unsigned int importantColorsStartByte = 50;   
};

#endif
