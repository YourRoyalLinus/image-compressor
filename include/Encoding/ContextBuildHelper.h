#ifndef CONTEXTBUILDHELPER_H
#define CONTEXTBUILDHELPER_H

#include "../Utils/Utils.h"
#include "../Artifacts/BMPImage.h"
#include "../Artifacts/PixelFrequencies.h"
#include "../Artifacts/Huffman/HuffmanTree.h"
#include "../Artifacts/Huffman/HuffmanTable.h"
#include "../Artifacts/Huffman/HuffmanTreeNode.h"
#include "./Huffman/HuffmanEncodingContext.h"
#include <assert.h>
#include <queue>
#include <algorithm>

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

        static std::unordered_map<int, std::string> CreateEncodingMap(int nonZeroNodes, PixelFrequencies* pixFreqs){
            std::unordered_map<int, std::string> temp;
            for(int pixelValue = 0; pixelValue < 256; pixelValue++){
                for(int k = 0; k < nonZeroNodes; k++){
                    if(pixelValue == pixFreqs[k].pix){
                        assert(pixFreqs[k].code != nullptr);
                        std::string c = pixFreqs[k].code;
                        temp[pixelValue] = c;
                        break;
                    }
                }
            }

            return temp;
        }

        static std::vector<std::string> CreateEncodedPixelVec(std::unordered_map<int, std::string> encodingMap,  int pixelDataArraySize, unsigned char* pixelDataArray) {
            int pixVal = 0;
            std::vector<std::string> encodedPixelVec;
            for(int i = 0; i < pixelDataArraySize; i++){
                pixVal = *(pixelDataArray+i);
                encodedPixelVec.push_back(encodingMap[pixVal]);                
            }

            return encodedPixelVec;
        }

        static int GetEncodedPixelDataSize(std::vector<std::string> encodedPixelVec){
            int s = 0;
            for(int i = 0; i < encodedPixelVec.size(); i++){
                s += encodedPixelVec[i].size();
            }
            
            return s;
        }

        static std::shared_ptr<HuffmanTreeNode> CreateHuffmanTreeNodes(PixelFrequencies* pixFreqs, HuffmanTree* tree, int totalNodes){
            std::vector<PixelFrequencies> test;
            for(unsigned i = 0; i < totalNodes; i++){
                std::cout << i << std::endl;
                test.push_back(pixFreqs[i]);

            }
            std::sort(test.begin(), test.end(), Compare);
            pixFreqs = &test[0];
            for(int i = 0; i < totalNodes; i++){
                std::cout << i << " NODE " << pixFreqs[i].pix << " FREQ " << pixFreqs[i].freq << std::endl;
                if(pixFreqs[i].left != nullptr){
                    std::cout << i << " LEFT CHILD " << pixFreqs[i].left->pix << " FREQ " << pixFreqs[i].left->freq << std::endl;
                }
                if(pixFreqs[i].right != nullptr){
                    std::cout << i << " LEFT RIGHT " << pixFreqs[i].right->pix << " FREQ " << pixFreqs[i].right->freq << std::endl;
                }
            }
            
            std::queue<std::shared_ptr<HuffmanTreeNode>> q;
            int node = 0;

            PixelFrequencies pixFreqNode;
            std::shared_ptr<HuffmanTreeNode> root = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(pixFreqs[node]));
            std::shared_ptr<HuffmanTreeNode> currentNode = root;
            
            q.push(root);
            while(node < totalNodes){
                pixFreqNode = pixFreqs[node];
                currentNode = q.front();
                q.pop();
                std::cout << "NODE: " << node << " PIX=" << currentNode->pix << " : " << currentNode->code << " - " << currentNode->freq << std::endl;
                std::cout << "PIXFREQ:" << node << " PIX=" << pixFreqNode.pix << " : " << pixFreqNode.code << " - " << pixFreqNode.freq << std::endl;
                if(currentNode->pix != pixFreqNode.pix){ // This is rife w/ potential for bugs
                    PixelFrequencies tmp = pixFreqNode;
                    pixFreqNode = pixFreqs[node+1];
                    pixFreqs[node+1] = tmp;
                     //Gotta clean this shit up
                    std::cout << "SWAPPING " << tmp.pix << " & " << pixFreqNode.pix << std::endl;
                    std::cout << "NEW SETUP:\n\n" ;
                    std::cout << "NODE: " << node << " PIX=" << currentNode->pix << " : " << currentNode->code << " - " << currentNode->freq << std::endl;
                    std::cout << "PIXFREQ:" << node << " PIX=" << pixFreqNode.pix << " : " << pixFreqNode.code << " - " << pixFreqNode.freq << std::endl;
                }

                if(pixFreqNode.left != nullptr){
                    currentNode->left = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixFreqNode.left));
                    std::cout << "NODE LEFT " << currentNode->left->code << std::endl;
                    q.push(currentNode->left);
                }
                if(pixFreqNode.right != nullptr){
                    currentNode->right = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixFreqNode.right));
                    std::cout << "NODE RIGHT " << currentNode->right->code << std::endl;
                    q.push(currentNode->right);
                }
                node++;
                std::cout << std::endl;
            }
            std::cout << "REM = " << q.size() << std::endl;
            return root;       

        }
    private:
        static bool Compare(const PixelFrequencies& lhs, const PixelFrequencies& rhs){
            return lhs.freq > rhs.freq;
        }
};

#endif