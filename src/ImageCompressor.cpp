#include "ImageCompressor.h"
#include "Utils.h"
#include <bitset>

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
    // Reading the BMP File
    file = fopen(bmpPath.c_str(), "rb");
    if (file == nullptr)
    {
        std::cout << "Error opening file" << bmpPath << std::endl;
        return;
    }
    else 
    { 
        // Extract the header data from the BMP File
        // FileSize will need to be recalculated
        file = fopen(bmpPath.c_str(), "rb");
        fread(headerBytes, sizeof(unsigned char), 54, file);
        // Get BPP
        short int bpp = *(short int*)&headerBytes[28];
        //Check if BPP is greater than 8 so I dont have to build a color table :)
        if(bpp <= 8){
            std::cout << "Invalid BMP file: Bits per pixel must be greater than 8. File BPP = " << bpp << std::endl;
            exit(-1);
        }
        // Get Offset from headerData
        // Set File Stream point to Data OffSet
        int offset = *(int*)&headerBytes[10];
        fseek(file, offset, SEEK_SET);

        // Get Weight from headerData
        int width = *(int*)&headerBytes[18];
        // Get Height from headerData
        int height = *(int*)&headerBytes[22];

        //Creating Image array
        imageArray = new int*[height];
        for (int i = 0; i < height; i++)
        {
            imageArray[i] = new int[width];
        }
        
        // Reading the BMP File 
        // into Image Array
        int temp = 0; 
        int bitDepth = bpp/8;
        for (int i = 0; i < height; i++) 
        {
            for (int j = 0; j <width; j++)
            {
                fread(&temp, bitDepth, 1, file);
                
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

    for(unsigned i = 0; i < *(int*)&headerBytes[22]; i++){
        for(unsigned j = 0; j < *(int*)&headerBytes[18]; j++){
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

/*
    Encodes the huffman tree into an image file into a temporary binary file. Included header information for the file to be parsed later
*/
void ImageCompressor::EncodeCompressedImage(FileConverter::FileInfo *fileInfo, int nodes){
    //Encoding the image
    int pixVal;
    int pixelDataSize = 0;

    int height = *(int*)&headerBytes[22];
    int width = *(int*)&headerBytes[18];

    headerBytes[30] =  std::bitset<32>(3).to_ulong(); //BI_BITFIELDS
    headerBytes[10] = std::bitset<32>(54).to_ulong();

    fileInfo->encodedPath = fileInfo->relativeFilePath + fileInfo->fileName + "_encoded.bin";
    FILE* huffmanImage = fopen(fileInfo->encodedPath.c_str(), "wb");
    if (huffmanImage == nullptr)
    {
        std::cout << "Error opening file" << fileInfo->encodedPath << std::endl;
        return;
    }
  

    //image data is read from bottom left -> top right
    for(int r = height-1; r >= 0; r--){
        for(int c = 0; c < width; c++){
            pixVal = imageArray[r][c];
            for(unsigned i = 0; i < nodes; i++){
                if(pixVal == pixFreq[i].pix){
                    fprintf(huffmanImage, "%s", pixFreq[i].code);
                    pixelDataSize += strlen(pixFreq[i].code);
                }
            }
        }
    }

    //8 bits per 1 Byte
    //Char size is 1 Byte -> Each 0 or 1 in the huffman tree costs 1 Byte to represent, but when decoded, will represent 1 bit
    //Dividing by 8 gives us the number of Bytes the compressed huffman image represents
    int tmp = std::bitset<32>(54 + (pixelDataSize /8)).to_ulong();

    headerBytes[5] =  (tmp >> 24) & 0xFF;
    headerBytes[4] =  (tmp >> 16) & 0xFF;
    headerBytes[3] = (tmp >> 8) & 0xFF;
    headerBytes[2] = tmp & 0xFF;
    
    fseek(huffmanImage, 0, SEEK_SET);

    // Write file header and info header into txt file
    for(unsigned i = 0; i < 54; i++){
        fputc(headerBytes[i], huffmanImage);
    }
   
    fclose(huffmanImage);

}