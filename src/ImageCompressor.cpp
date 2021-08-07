#include "ImageCompressor.h"
#include "Utils.h"
#include <bitset>
#include <cassert>

ImageCompressor::ImageCompressor(){
}

ImageCompressor::~ImageCompressor(){
}

/*
    Compress an image file using Huffman encoding
*/
void ImageCompressor::CompressImageFile(FileConverter::FileInfo *fileInfo){
    ReadInto2DArray(fileInfo->bmpPath);

    int* hist = GetHistogram();
    int nodes = GetNonZeroOccurances(hist);
    float p = GetMinimumProb(hist);
    int totalNodes = GetTotalNodes(nodes);
    int maxCodeLen = GetMaxCodeLength(p);

    InitStructs(maxCodeLen, nodes, totalNodes);
    InitArrays(hist);

    SortHuffCodeArray(nodes); 
    BuildHuffmanTree(nodes);
    Backtrack(nodes, totalNodes);

    EncodeCompressedImage(fileInfo, nodes);
}

/*
    Reads the bmp image into a 2D array
*/
void ImageCompressor::ReadInto2DArray(std::string bmpPath){
    cimg_library::CImg<unsigned char> img(bmpPath.c_str());
    cimage = img;

    unsigned char* pixData = cimage.data();

    // Extract the header data from the BMP File
    // FileSize will need to be recalculated
    file = fopen(bmpPath.c_str(), "rb");
    if(file == nullptr){
        std::cout << "Error opening " << bmpPath << std::endl;
        return;
    }

    fread(headerBytes, sizeof(unsigned char), 54, file);
    // Get BPP
    short int bpp = *(short int*)&headerBytes[28];
    //Check if BPP is greater than 8 so I dont have to build a color table :)
    if(bpp <= 8){
        std::cout << "Invalid BMP file: Bits per pixel must be greater than 8. File BPP = " << bpp << std::endl;
        exit(-1);
    }
    
    // Get Weight from headerData
    int width = *(int*)&headerBytes[18];
    // Get Height from headerData
    int height = *(int*)&headerBytes[22];

    pixelDataArray = cimage.data();

    TestEncoding(bmpPath);
}

/*
    Create the histogram of the pixel intensity values present in the image
*/
int* ImageCompressor::GetHistogram(){
    int* hist = new int[256];
    for(unsigned i = 0; i < 256; i++){
        hist[i] = 0;
    }

    for(unsigned i = 0; i < cimage.size(); i++){
        int pixIntensityValue = *(pixelDataArray+i);
        hist[pixIntensityValue] += 1;
    }

    return hist;
}

int ImageCompressor::GetNonZeroOccurances(int* hist){
    int nodes = 0;
    for(unsigned i = 0; i < 256; i++){
        if(hist[i] != 0){
            nodes++;
        }
    }
    return nodes;
}

/*
    Calculates the max code length of Huffman code word
*/
int ImageCompressor::GetMaxCodeLength(float p){
    int i = 0;
    while((1/p) > Utils::Fib(i)){
        i++;
    }

    return (i - 3); //max code length
}

/*
    Calculates the total number of nodes in the Huffman tree
*/
int ImageCompressor::GetTotalNodes(int nodes){
    return 2 * nodes - 1;
}

/*
    Calculate the minimum probability
*/
float ImageCompressor::GetMinimumProb(int* hist){
    float p = 1.0;
    float pTemp;

    for(unsigned i = 0; i < 256; i++){
        pTemp = (hist[i] / (float) ( (*(int*)&headerBytes[22]) * (*(int*)&headerBytes[18]) ));
        if(pTemp > 0 && pTemp <= p){
            p = pTemp;
        }
    }

    return p;
}
/*
    Initialize the PixFreq and HuffCodes structures
*/
void ImageCompressor::InitStructs(int mcl, int nodes, int totalNodes){
    pixFreq = new PixFreq[totalNodes];
    huffCode = new HuffCode[nodes];
    for(unsigned i = 0; i < totalNodes; i++){
        pixFreq[i] = PixFreq(mcl);
    }
    huffTable = new HuffTable();
}

/*
    Initialize the two arrays pixFreq and huffCodes with information of the leaf nodes
*/
void ImageCompressor::InitArrays(int* hist){
    int totpix = (*(int*)&headerBytes[22]) * (*(int*)&headerBytes[18]);
    float tempProb;
    int j = 0;

    for(unsigned i = 0; i < 256; i++){
        if(hist[i] != 0){

            // pixel intensity value
            huffCode[j].pix = i; 
            pixFreq[j].pix = i;
            
            // location of the node in the pixFreq array
            huffCode[j].arrloc = j; 
            
            // probability of occurrence
            tempProb = (float)hist[i] / (float)totpix; 
            pixFreq[j].freq = tempProb;
            huffCode[j].freq = tempProb;
            
            // Declaring the child of leaf node as NULL pointer
            pixFreq[j].left = nullptr; 
            pixFreq[j].right = nullptr;
            
            // initializing the code
            // word as end of line
            pixFreq[j].code[0] = '\0'; 
            j++;
        }
    }
}

/*
    Sort the huffCode array according to the probability of occurance of the pixel intensity value
*/
void ImageCompressor::SortHuffCodeArray(int nodes){
    // Sorting the histogram
    struct HuffCode temphuff;
    
    // Sorting w.r.t probability of occurrence
    for (int i = 0; i < nodes; i++)
    {
        for (int j = i + 1; j < nodes; j++) 
    {
            if (huffCode[i].freq < huffCode[j].freq) 
    {
                temphuff = huffCode[i];
                huffCode[i] = huffCode[j];
                huffCode[j] = temphuff;
            }
        }
    }
}

/*
    Build the Huffman Tree
*/
void ImageCompressor::BuildHuffmanTree(int nodes){
    float sumProb;
    int sumPix;
    int n = 0;
    int nextNode = nodes;

    // Total number of nodes in Huffman Tree is 2*nodes-1
    while (n < nodes - 1) 
    {
        
        // Adding the lowest two probabilities
        sumProb = huffCode[nodes - n - 1].freq + huffCode[nodes - n - 2].freq;
        sumPix = huffCode[nodes - n - 1].pix + huffCode[nodes - n - 2].pix;
        
        // Appending to the pixFreq Array
        pixFreq[nextNode].pix = sumPix;
        pixFreq[nextNode].freq = sumProb;
        pixFreq[nextNode].left = &pixFreq[huffCode[nodes - n - 2].arrloc];
        
        // arrloc points to the location of the child node in the pixFreq array
        pixFreq[nextNode].right = &pixFreq[huffCode[nodes - n - 1].arrloc];
        pixFreq[nextNode].code[0] = '\0';
        
        // Using sum of the pixel values as  new representation for the new node since unlike strings, we cannot 
        // concatenate because the pixel values are stored as integers. However, if we store the pixel values as strings
        // we can use the concatenated string as a representation of the new node.
        int i = 0;
        
        // Sorting and Updating the huffCode array simultaneously New position of the combined node
        while (sumProb <= huffCode[i].freq)
            i++;
            
        // Inserting the new node in the huffCode array
        for (int k = nodes; k >= 0; k--) 
        {
            if (k == i)
            {
                huffCode[k].pix = sumPix;
                huffCode[k].freq = sumProb;
                huffCode[k].arrloc = nextNode;
            }
            else if (k > i)
            
                // Shifting the nodes below the new node by 1 For inserting the new node at the updated position k
                huffCode[k] = huffCode[k - 1];
        }
        n += 1;
        nextNode += 1;
    }
}

/*
    Backtrack from the root to the leaf nodes to assign code words
*/
void ImageCompressor::Backtrack(int nodes, int totalNodes){
    char left = '0';
    char right = '1';

    for(unsigned i = totalNodes - 1; i >= nodes; i--){
        if(pixFreq[i].left != nullptr){
            ConcatCodes(pixFreq[i].left->code, pixFreq[i].code, left);
        }
        if(pixFreq[i].right != nullptr){
            ConcatCodes(pixFreq[i].right->code, pixFreq[i].code, right);
        }
    }
}

/*
    Concatonate the code for each node until you reach the first parent node: 
    EG: 0, 0, 1, 0, 0, 1, \0 -> 001001
*/
void ImageCompressor::ConcatCodes(char* str, char* parentCode, char add){
    int i = 0;
    while (*(parentCode + i) != '\0') 
    {
        *(str + i) = *(parentCode + i);
        i++;
    }
    if (add != '2') 
    {
        str[i] = add;
        str[i + 1] = '\0';
    }
    else
        str[i] = '\0';
}

int ImageCompressor::EncodeBits(char* bits){
    int byte = std::bitset<32>(bits).to_ulong();
    return byte;
}

/*
    Encodes the huffman tree into an image file into a temporary binary file. Included header information for the file to be parsed later
*/
void ImageCompressor::EncodeCompressedImage(FileConverter::FileInfo *fileInfo, int nodes){
    //Encoding the image
    int pixVal;
    int pixelDataSize = 0;
    int pixelDataArraySize = 0;

    fileInfo->encodedPath = fileInfo->relativeFilePath + fileInfo->fileName + "_encoded.bin";
    FILE* huffmanImage = fopen(fileInfo->encodedPath.c_str(), "wb");
    if (huffmanImage == nullptr)
    {
        std::cout << "Error opening file" << fileInfo->encodedPath << std::endl;
        return;
    }
    //std::stringstream pixelDataArrayStringStream;
    for(unsigned i = 0; i < cimage.size(); i++){
        pixVal = *(pixelDataArray+i);
        for(unsigned k = 0; k < nodes; k++){
            if(pixVal == pixFreq[k].pix){
                unsigned short int codeSize = strlen(pixFreq[k].code);
                huffTable->codeLengths.push_back(codeSize);
                huffTable->codes.push_back(pixFreq[k].code);
                /*
                int code = EncodeBits(pixFreq[k].code);
                pixelDataSize += 6;
                pixelDataArraySize += codeSize;
                pixelDataArrayStringStream.write(std::to_string(codeSize).c_str(), 1); //TEST
                pixelDataArrayStringStream.write(std::to_string(code).c_str(), 1); //TEST
                */
                //fprintf(huffmanImage, "%d", codeSize);
                //fprintf(huffmanImage, "%d", code);
            }
        }
    }

    CreateHuffTable(nodes);
    int height = *(int*)&headerBytes[22];
    int width = *(int*)&headerBytes[18];

    int headerSize = 58;
    int dhtSerializedSize = SerializeHuffTable(fileInfo->encodedPath, 58);
    int pixelDataOffset = headerSize+dhtSerializedSize;

    int encodedFileSize = std::bitset<32>(headerSize + dhtSerializedSize).to_ulong();

    headerBytes[30] =  std::bitset<32>(3).to_ulong(); //BI_BITFIELDS
    
    headerBytes[13] = (pixelDataOffset >> 24) & 0xFF;
    headerBytes[12] = (pixelDataOffset >> 16) & 0xFF;
    headerBytes[11] = (pixelDataOffset >> 8) & 0xFF;
    headerBytes[10] = pixelDataOffset & 0xFF;

    headerBytes[5] =  (encodedFileSize >> 24) & 0xFF;
    headerBytes[4] =  (encodedFileSize >> 16) & 0xFF;
    headerBytes[3] = (encodedFileSize >> 8) & 0xFF;
    headerBytes[2] = encodedFileSize & 0xFF;

    unsigned char fileMetaData[4]; //????
    fileMetaData[3] =  (pixelDataOffset >> 24) & 0xFF;
    fileMetaData[2] =  (pixelDataOffset >> 16) & 0xFF;
    fileMetaData[1] = (pixelDataOffset >> 8) & 0xFF;
    fileMetaData[0] = pixelDataOffset & 0xFF;

    
    fseek(huffmanImage, 0, SEEK_SET);
    
    // Write file header and info header into txt file
    for(unsigned i = 0; i < 54; i++){
        fputc(headerBytes[i], huffmanImage);
    }
    for(unsigned i = 0; i < 4; i++){
        fputc(fileMetaData[i], huffmanImage);
    }
    
    fclose(huffmanImage);
    
}

void ImageCompressor::DecompressImageFile(FileConverter::FileInfo* fileInfo){
    int height = 0;
    int width = 0;
    long offset = 0;
    int fileSize = 0;
    int dataArraySize = 0;
    unsigned char header[58];

    FILE* encodedFile = fopen(fileInfo->encodedPath.c_str(), "rb");
    if (encodedFile == nullptr)
    {
        std::cout << "Error opening encoded file" << fileInfo->encodedPath << std::endl;
        return;
    }

    // Reader header data into the header array
    fread(header, sizeof(unsigned char), 58, encodedFile);
    fileSize = *(int*)&header[2];
    offset = *(int*)&header[10];
    width = *(int*)&header[18];
    height = *(int*)&header[22];
    dataArraySize = *(int*)&header[54];
    int dhtBitStreamSize = offset-58; 

    std::cout << "FILESIZE=" << fileSize << std::endl;
    std::cout << "OFFSET=" << offset << std::endl;
    std::cout << "WIDTH=" << width << std::endl;
    std::cout << "HEIGHT=" << height << std::endl;
    std::cout << "DHT DECODED SIZE = " << dhtBitStreamSize << std::endl; 
    std::cout << "DATA ARRAY SIZE = " << dataArraySize << std::endl;

    // Move stream pointer to the start of the data 
    //NEED TO STORE SIZE OF INFO AFTER HEADER DATA???
    //fseek(encodedFile, 58, SEEK_SET);
    
    //Read in DHT
    DeserializeHuffTable(fileInfo->encodedPath, 58, offset);
    //Read in DHT tree codes and convert back to pixel values
    unsigned char* pixArr = new unsigned char[newHuffTable.codes.size()];
    std::vector<unsigned char> pixelArray(newHuffTable.codes.size());
    for(unsigned i = 0; i < newHuffTable.codes.size(); i++){
        std::string encodedPixelCode = newHuffTable.codes[i];
        int decodedPixelValue = newHuffTable.table[encodedPixelCode];
        pixArr[i] = decodedPixelValue & 0xFF;
        //std::string decodedPixelValuesBits = std::bitset<sizeof(unsigned char)>(decodedPixelValue).to_string();
        std::cout << "ENCODED CODE = " << encodedPixelCode << " AT POS =" << i << " = " << decodedPixelValue << " STORED AS =" << (decodedPixelValue & 0xFF) << std::endl;
    }

    /*
    unsigned char* pixelArray = new unsigned char[dataArraySize+1];  
    int pixArrayPos = 0;
    int codeLength = GetCodeLength(encodedFile);
    char* code = DecodeBits(encodedFile, codeLength);
    while(code != nullptr){
        for(unsigned i = 0; i < codeLength; i++){
            std::cout << "ENCODED CODE = " << code << "AT POS " << pixArrayPos << " = " << huffTable->table[code] << std::endl;
            pixelArray[pixArrayPos] = huffTable->table[code];
            pixArrayPos++;
        }
        codeLength = GetCodeLength(encodedFile);
        code = DecodeBits(encodedFile, codeLength);
    }
    */

    TestDecoding(pixArr, width, height);

    return;
}

char* ImageCompressor::DecodeBits(FILE* encodedFile, int significantBits){
    char* encodedBits;
    char* decodedBits;
    const char* bitStream;
    std::string bitSeq = 0;
    int streamReaderRes = 0;
    
    streamReaderRes = fread(encodedBits, sizeof(int), 1, encodedFile);
    if(streamReaderRes == 0){
        return nullptr;
    }
    
    bitSeq = std::bitset<32>(encodedBits).to_string();
    bitStream = bitSeq.substr(sizeof(int) - significantBits).c_str();

    decodedBits = new char[significantBits];
    for(unsigned i = 0; i < significantBits; i++){
        decodedBits[i] = bitStream[i];
    }

    std::cout << "CODE = " << std::string(decodedBits) << std::endl;
    return decodedBits;
}

unsigned short int ImageCompressor::GetCodeLength(FILE* encodedFile){
    unsigned short int* size = 0;
    fread(size, sizeof(unsigned short int), 1, encodedFile);
    std::cout << "CODE LENGTH = " << *size << std::endl;
    return *size;
}

void ImageCompressor::CreateHuffTable(int nodes){
    std::unordered_map<std::string, int> temp;
    for(unsigned pixelValue = 0; pixelValue < 256; pixelValue++){
        for(unsigned k = 0; k < nodes; k++){
            if(pixelValue == pixFreq[k].pix){
                assert(pixFreq[k].code != nullptr);
                //std::cout << "KEY CODE = " << pixFreq[k].code << " REPRESENTS PIXELVALUE = " << pixelValue << std::endl;
                std::string c = pixFreq[k].code;
                temp[c] = pixelValue;
                break;
            }
        }
    }

    huffTable->table = temp;
}

int ImageCompressor::SerializeHuffTable(std::string encodedFilePath, int offset){
    std::ofstream serializedOutputFile = std::ofstream(encodedFilePath, std::ofstream::binary);
    serializedOutputFile.seekp(offset);
    struct HuffTable tmpHuffTable = *huffTable;
    {
        cereal::BinaryOutputArchive binaryOutputArchive(serializedOutputFile);
        binaryOutputArchive(tmpHuffTable);
    }
    int end = serializedOutputFile.tellp();

    return end - offset;
}

void ImageCompressor::DeserializeHuffTable(std::string encodedFilePath, int offset, int end){
    struct HuffTable tmpHuffTable;
    std::ifstream serializedInputFile = std::ifstream(encodedFilePath, std::ifstream::binary);
    serializedInputFile.seekg(offset);
    {
        cereal::BinaryInputArchive binaryInputArchive(serializedInputFile);
        binaryInputArchive(tmpHuffTable);
    }
    
    newHuffTable = tmpHuffTable;
    std::cout << "TESTING scope =" << huffTable->codes[0] << std::endl;

}
void ImageCompressor::TestEncoding(std::string s){
    cimg_library::CImg<unsigned char> image(s.c_str());
    unsigned char* pixData = image.data();
    cimg_library::CImg<unsigned char> newImage(pixData, image.width(), image.height(), 1, 3);
    newImage.save_bmp("/home/jon/ImageCompressor/test.bmp");
}

void ImageCompressor::TestDecoding(unsigned char* pixelArr, int width, int height){
    cimg_library::CImg<unsigned char> newImage(pixelArr, width, height, 1, 3);
    newImage.save_bmp("/home/jon/ImageCompressor/test_decoding.bmp");
}