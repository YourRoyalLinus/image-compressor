#ifndef CONTEXTBUILDHELPER_H
#define CONTEXTBUILDHELPER_H

#include "Utils.h"
#include "BMPImage.h"
#include "PixelFrequencies.h"
#include "HuffmanTree.h"
#include "HuffmanTable.h"
#include "HuffmanEncodingContext.h"
#include <assert.h>

class ContextBuildHelper{
    public:
        static int* GetHistogram(int pixelBufferSize, unsigned char* pixelDataArray ){
            int* hist = new int[256];
            for(unsigned i = 0; i < 256; i++){
                hist[i] = 0;
            }

            for(unsigned i = 0; i < pixelBufferSize; i++){
                int pixIntensityValue = *(pixelDataArray+i);
                hist[pixIntensityValue] += 1;
            }

            return hist;
        }

        static int GetNonZeroOccurances(int* hist){
            int nodes = 0;
            for(unsigned i = 0; i < 256; i++){
                if(hist[i] != 0){
                    nodes++;
                }
            }

            return nodes;
        }

        static int GetMaxCodeLength(float p){
            int i = 0;
            while((1/p) > Utils::FibonacciSeq(i)){
                i++;
            }

            return (i - 3);
        }

        static int GetTotalNodes(int nodes){
            return 2 * nodes - 1;
        }

        static float GetMinimumProbabilityOfOccurance(int* hist, int imageWidth, int imageHeight){
            float probability = 1.0;
            float probabilityTemp;

            for(unsigned i = 0; i < 256; i++){
                probabilityTemp = (hist[i] / (float) ( imageHeight * imageWidth ));
                if(probabilityTemp > 0 && probabilityTemp <= probability){
                    probability = probabilityTemp;
                }
            }

            return probability;
            
        }

        static PixelFrequencies* InitializePixelFrequencies(int totalNodes, int maxCodeLength){
            PixelFrequencies* pf = new PixelFrequencies[totalNodes];
            for(unsigned i = 0; i < totalNodes; i++){
                pf[i] = PixelFrequencies(maxCodeLength);
            }
     
            return pf;
        }

        static HuffmanTree* InitializeHuffmanTree(int nonZeroNodes){
            HuffmanTree* ht = new HuffmanTree[nonZeroNodes];
            return ht;
        }

        static void InitializeLeafNodes(int* hist, int totalPixels, PixelFrequencies* pixFreqs, HuffmanTree* huffTree){
            float tempProb;
            int j = 0;

            for(unsigned i = 0; i < 256; i++){
                if(hist[i] != 0){

                    // pixel intensity value
                    huffTree[j].pix = i; 
                    pixFreqs[j].pix = i;
                    
                    // location of the node in the pixFreq array
                    huffTree[j].arrloc = j; 
                    
                    // probability of occurrence
                    tempProb = (float)hist[i] / (float)totalPixels; 
                    pixFreqs[j].freq = tempProb;
                    huffTree[j].freq = tempProb;
                    
                    // Declaring the child of leaf node as NULL pointer
                    pixFreqs[j].left = nullptr; 
                    pixFreqs[j].right = nullptr;
                    
                    // initializing the code
                    // word as end of line
                    pixFreqs[j].code[0] = '\0'; 
                    j++;
                }
            }
        }
        
        // Sorting w.r.t probability of occurrence
        static void SortHuffCodeArray(int nonZeroNodes, HuffmanTree* huffTree){
            HuffmanTree tempHuff;
            
            for (int i = 0; i < nonZeroNodes; i++)
            {
                for (int j = i + 1; j < nonZeroNodes; j++) 
                {
                    if (huffTree[i].freq < huffTree[j].freq) 
                    {
                        tempHuff = huffTree[i];
                        huffTree[i] = huffTree[j];
                        huffTree[j] = tempHuff;
                    }
                }
            }
        }

        static void CreateHuffmanTree(int nonZeroNodes, PixelFrequencies* pixFreqs, HuffmanTree* huffTree){
            float sumProb;
            int sumPix;
            int n = 0;
            int nextNode = nonZeroNodes;

            while (n < nonZeroNodes - 1) 
            {
                // Adding the lowest two probabilities
                sumProb = huffTree[nonZeroNodes - n - 1].freq + huffTree[nonZeroNodes - n - 2].freq;
                sumPix = huffTree[nonZeroNodes - n - 1].pix + huffTree[nonZeroNodes - n - 2].pix;
                
                // Appending to the pixFreq Array
                pixFreqs[nextNode].pix = sumPix;
                pixFreqs[nextNode].freq = sumProb;
                pixFreqs[nextNode].left = &pixFreqs[huffTree[nonZeroNodes - n - 2].arrloc];
                
                // arrloc points to the location of the child node in the pixFreq array
                pixFreqs[nextNode].right = &pixFreqs[huffTree[nonZeroNodes - n - 1].arrloc];
                pixFreqs[nextNode].code[0] = '\0';

                // Using sum of the pixel values as  new representation for the new node since unlike strings, we cannot 
                // concatenate because the pixel values are stored as integers. However, if we store the pixel values as strings
                // we can use the concatenated string as a representation of the new node.
                int i = 0;
                
                // Sorting and Updating the huffCode array simultaneously New position of the combined node
                while (sumProb <= huffTree[i].freq)
                    i++;
                    
                // Inserting the new node in the huffCode array
                for (int k = nonZeroNodes; k >= 0; k--) 
                {
                    if (k == i)
                    {
                        huffTree[k].pix = sumPix;
                        huffTree[k].freq = sumProb;
                        huffTree[k].arrloc = nextNode;
                    }
                    else if (k > i)
                    
                        // Shifting the nodes below the new node by 1 For inserting the new node at the updated position k
                        huffTree[k] = huffTree[k - 1];
                }
                n += 1;
                nextNode += 1;
            }
        }
        
        static void Backtrack( int nodes, int totalNodes, PixelFrequencies* pixFreqs){
            const char left = '0';
            const char right = '1';

            for(unsigned i = totalNodes - 1; i >= nodes; i--){
                if(pixFreqs[i].left != nullptr){
                    ConcatCodes(pixFreqs[i].left->code, pixFreqs[i].code, left);
                }
                if(pixFreqs[i].right != nullptr){
                    ConcatCodes(pixFreqs[i].right->code, pixFreqs[i].code, right);
                }
            }
        }

        static void ConcatCodes(char* str, char* parentCode, char add){
            int i = 0;
            while (*(parentCode + i) != '\0'){
                *(str + i) = *(parentCode + i);
                i++;
            }

            if (add != '2'){
                str[i] = add;
                str[i + 1] = '\0';
            }
            else{
                str[i] = '\0';
            } 
        }

        static HuffmanTable* CreateHuffmanTable(int nonZeroNodes, PixelFrequencies* pixFreqs){
            HuffmanTable* ht = new HuffmanTable();
            std::unordered_map<std::string, int> temp;
            for(unsigned pixelValue = 0; pixelValue < 256; pixelValue++){
                for(unsigned k = 0; k < nonZeroNodes; k++){
                    if(pixelValue == pixFreqs[k].pix){
                        assert(pixFreqs[k].code != nullptr);
                        std::string c = pixFreqs[k].code;
                        temp[c] = pixelValue;
                        break;
                    }
                }
            }

            ht->table = temp;

            return ht;
        }

        static void PopulateCodeLengths(int nonZeroNodes, int imageSize, unsigned char* pixelDataArray, PixelFrequencies* pixFreqs, HuffmanTable* huffTable){
            int pixVal = 0;
             for(unsigned i = 0; i < imageSize; i++){
                pixVal = *(pixelDataArray+i);
                for(unsigned k = 0; k < nonZeroNodes; k++){
                    if(pixVal == pixFreqs[k].pix){
                        InsertCodeLengths(k, pixFreqs, huffTable); 
                    }
                }
            }
            
        }
    private:
        static void InsertCodeLengths(int node, PixelFrequencies* pixFreqs, HuffmanTable* huffTable){
            unsigned short int codeSize = strlen(pixFreqs[node].code);
            huffTable->codeLengths.push_back(codeSize);
            huffTable->codes.push_back(pixFreqs[node].code);
        }
};

#endif