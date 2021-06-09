#include "ImageCompressor.h"
#include "Utils.h"
#include <bitset>

ImageCompressor::ImageCompressor(){
    bmpHeader = new Header();
    bmpInfoHeader = new InfoHeader();
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
    int data = 0;
    int offset = 0;
    int bpp = 0;
    long bmpsize = 0;
    int bmpdataoff = 0;

    // Reading the BMP File
    file = fopen(bmpPath.c_str(), "rb");
    if (file == nullptr)
    {
        std::cout << "Error opening file" << bmpPath << std::endl;
        return;
    }
    else 
    {
        // Set file position of the 
        // stream to the beginning
        // Contains file signature 
        // or ID "BM"
        int offset = 0;
        
        // Set offset to 2, which 
        // contains size of BMP File
        offset = 2;
        
        fseek(file, offset, SEEK_SET);
        
        // Getting size of BMP File
        fread(&bmpHeader->fileSize, 4, 1, file);

        fread(&bmpHeader->reserved, 4, 1, file);

        // Get offset to start of bmp data
        fread(&bmpHeader->dataOffset, 4, 1, file);
        
        /*
        
        // Getting offset where the
        // pixel array starts
        // Since the information 
        // is at offset 10 from 
        // the start, as given 
        // in BMP Header
        offset = 10; 
        
        fseek(file, offset, SEEK_SET); 
        
        // Bitmap data offset
        fread(&bmpdataoff, 4, 1, file); 
        
        */
        
        // Getting height and width of the image
        // Width is stored at offset 18 and height
        // at offset 22, each of 4 bytes
        fseek(file, 18, SEEK_SET);
        
        fread(&bmpInfoHeader->width, 4, 1, file);
        
        fread(&bmpInfoHeader->height, 4, 1, file);
        
        // Number of bits per pixel
        fseek(file, 2, SEEK_CUR);
        
        fread(&bmpInfoHeader->bitsPerPixel, 2, 1, file);

        if(bmpInfoHeader->bitsPerPixel <= 8){
            std::cout << "Invalid BMP file: Bits per pixel must be greater than 8. File BPP = " << bmpInfoHeader->bitsPerPixel << std::endl;
            exit(-1);
        }
        
        fseek(file, 54, SEEK_SET);
        //GET DATA BETWEEN THIS AND OFFSET
        
        // Setting offset to start of pixel data
        fseek(file, bmpHeader->dataOffset, SEEK_SET);
         
        // Creating Image array
        imageArray = new int*[bmpInfoHeader->height];
        for (int i = 0; i < bmpInfoHeader->height; i++)
        {
            imageArray[i] = new int[bmpInfoHeader->width];
        }
        
        // Reading the BMP File 
        // into Image Array
        int temp = 0;
        for (int i = 0; i < bmpInfoHeader->height; i++) 
        {
            for (int j = 0; j < bmpInfoHeader->width; j++)
            {
                fread(&temp, 3, 1, file);
                
                // the Image is a 
                // 24-bit BMP Image
                temp = temp & 0x0000FF; 
                imageArray[i][j] = temp;
            }
        }
    }
}

/*
    Create the histogram of the pixel intensity values present in the image
*/
int* ImageCompressor::GetHistogram(){
    int* hist = new int[256];
    for(unsigned i = 0; i < 256; i++){
        hist[i] = 0;
    }

    for(unsigned i = 0; i < bmpInfoHeader->height; i++){
        for(unsigned j = 0; j < bmpInfoHeader->width; j++){
            hist[imageArray[i][j]] += 1;
        }
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
        pTemp = (hist[i] / (float)(bmpInfoHeader->height * bmpInfoHeader->width));
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
}

/*
    Initialize the two arrays pixFreq and huffCodes with information of the leaf nodes
*/
void ImageCompressor::InitArrays(int* hist){
    int totpix = bmpInfoHeader->height* bmpInfoHeader->width;
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
    unsigned i = 0;
    while(*(parentCode+i) != '\0'){
        *(str+i) = *(parentCode + i);
        i++;
    }
    str[i] = add;
    str[i+1] = '\0';
}

/*
    Encodes the huffman tree into an image file into a temporary text file. Saves the tmpFile to fileInfo
*/
void ImageCompressor::EncodeCompressedImage(FileConverter::FileInfo *fileInfo, int nodes){
    //Encoding the image
    int pixVal;
    std::string pixelData;
    
    // Encode Pixel Data
    for(unsigned r = 0; r < bmpInfoHeader->height; r++){
        for(unsigned c = 0; c < bmpInfoHeader->width; c++){
            pixVal = imageArray[r][c];
            for(unsigned i = 0; i < nodes; i++){
                if(pixVal == pixFreq[i].pix){
                    pixelData += pixFreq[i].code;
                }
            }
        }
    }
    bmpHeader->dataOffset = 54;
    bmpHeader->fileSize =  bmpHeader->dataOffset + (pixelData.size() / 4);

    std::vector<std::string> headerVect(16);
    //Encode the File Header
    std::string sSignature = bmpHeader->signature;
    std::string sFileSize = std::bitset<32>(bmpHeader->fileSize).to_string();
    std::string sReservedHeader= std::bitset<32>(bmpHeader->reserved).to_string();
    std::string sDataOffset = std::bitset<32>(bmpHeader->dataOffset).to_string(); //Using old file info??

    headerVect.push_back(sSignature);
    headerVect.push_back(sFileSize);
    headerVect.push_back(sReservedHeader);
    headerVect.push_back(sDataOffset);

    //Encode the InfoHeader
    std::string sSize = std::bitset<32>(bmpInfoHeader->headerSize).to_string();
    std::string sWidth = std::bitset<32>(bmpInfoHeader->width).to_string();
    std::string sHeight = std::bitset<32>(bmpInfoHeader->height).to_string();
    std::string sPlanes = std::bitset<8>(bmpInfoHeader->planes).to_string();
    std::string sBPP = std::bitset<8>(bmpInfoHeader->bitsPerPixel).to_string();
    std::string sCompression = std::bitset<32>(bmpInfoHeader->compression).to_string();
    std::string sImgSize = std::bitset<32>(bmpInfoHeader->imageSize).to_string();
    std::string sXPixels = std::bitset<32>(bmpInfoHeader->xPixelsPerM).to_string();
    std::string sYPixels = std::bitset<32>(bmpInfoHeader->yPixelsPerM).to_string();
    std::string sColorsUsed = std::bitset<32>(bmpInfoHeader->colorsUsed).to_string();
    std::string sImportantColors = std::bitset<32>(bmpInfoHeader->importantColors).to_string();

    headerVect.push_back(sSize);
    headerVect.push_back(sWidth);
    headerVect.push_back(sHeight);
    headerVect.push_back(sPlanes);
    headerVect.push_back(sBPP);
    headerVect.push_back(sCompression);
    headerVect.push_back(sImgSize);
    headerVect.push_back(sXPixels);
    headerVect.push_back(sYPixels);
    headerVect.push_back(sColorsUsed);
    headerVect.push_back(sImportantColors);

    headerVect.push_back(pixelData);

    fileInfo->tmpPath = fileInfo->relativeFilePath + fileInfo->fileName + ".txt";
    
    FILE* huffmanImage = fopen(fileInfo->tmpPath.c_str(), "wb");

    // Write file header and info header into txt file
    for(unsigned i = 0; i < headerVect.size(); i++){
        fprintf(huffmanImage, "%s", headerVect[i].c_str());
    }
    
    fclose(huffmanImage);

}