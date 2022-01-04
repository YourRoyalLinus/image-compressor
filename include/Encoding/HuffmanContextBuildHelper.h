#ifndef HUFFMANCONTEXTBUILDHELPER_H
#define HUFFMANCONTEXTBUILDHELPER_H

#include "../Artifacts/PixelFrequencies.h"
#include "../Artifacts/Huffman/HuffmanTree.h"
#include "../Artifacts/Huffman/HuffmanTreeNode.h"
#include <unordered_map>
#include <vector>
#include <assert.h>
#include <queue>
#include <algorithm>
#include <memory>

namespace ContextBuilder{
    namespace {
        bool CompareFrequencies(const PixelFrequencies& lhs, const PixelFrequencies& rhs){
            return lhs.freq > rhs.freq;
        }

        std::vector<PixelFrequencies> SortPixFrequencies(int totalNodes, PixelFrequencies* pixFreqs){
            std::vector<PixelFrequencies> tmp;
            for(unsigned i = 0; i < totalNodes; i++){
                tmp.push_back(pixFreqs[i]);
            }
            std::sort(tmp.begin(), tmp.end(), CompareFrequencies);

            return tmp;
        }

        /* 
        When two nodes in the tree have the same frequency, their ordering between the PixFreqs array and
        the BFS-based tree can be swapped (left node in one  == right node in the other).
        This happens more frequently with nodes deeper in the tree.
        */
        void SwapChildNodes(int node, PixelFrequencies& currentPixFreqNode, PixelFrequencies* pixFreqs){
            PixelFrequencies tmp = currentPixFreqNode;
            currentPixFreqNode = pixFreqs[node+1];
            pixFreqs[node+1] = tmp;
        }
    }

    PixelFrequencies* InitializePixelFrequencies(int totalNodes, int maxCodeLength){ //rewrite all of this absolute garbage 
        PixelFrequencies* pf = new PixelFrequencies[totalNodes];
        for(unsigned i = 0; i < totalNodes; i++){
            pf[i] = PixelFrequencies(maxCodeLength);
        }
    
        return pf;
    }

    HuffmanTree* InitializeHuffmanTree(int nonZeroNodes){
        HuffmanTree* ht = new HuffmanTree[nonZeroNodes];
        for(unsigned i = 0; i < nonZeroNodes; i++){
            ht[i] = HuffmanTree();
        }
        return ht;      
    }

    void InitializeLeafNodes(const std::vector<int>& hist, int totalPixels, PixelFrequencies* pixFreqs, HuffmanTree* huffTree){
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
    void SortHuffCodeArray(int nonZeroNodes, HuffmanTree* huffTree){
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

    void CreateHuffmanTree(int nonZeroNodes, PixelFrequencies* pixFreqs, HuffmanTree* huffTree){
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

    void ConcatCodes(char* str, char* parentCode, char add){
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
    
    void Backtrack(int nodes, int totalNodes, PixelFrequencies* pixFreqs){
        const char left = '0';
        const char right = '1';

        for(unsigned i = totalNodes - 1; i >= nodes; i--){
            if(pixFreqs[i].left != nullptr){
                ConcatCodes(pixFreqs[i].left->code.get(), pixFreqs[i].code.get(), left);
            }
            if(pixFreqs[i].right != nullptr){
                ConcatCodes(pixFreqs[i].right->code.get(), pixFreqs[i].code.get(), right);
            }
        }
    }

    std::string GetHuffmanCode(int pixel, int nonZeroNodes, PixelFrequencies* pixFreqs){
        for(int k = 0; k < nonZeroNodes; k++){
            if(pixel == pixFreqs[k].pix){
                return std::string(pixFreqs[k].code.get());
            }
        }
    
        return "";
    }
    

    std::shared_ptr<HuffmanTreeNode> CreateHuffmanTreeNodes(PixelFrequencies* pixFreqs, int totalNodes){
        std::vector<PixelFrequencies> sortedPixelFreqVec = SortPixFrequencies(totalNodes, pixFreqs);
        pixFreqs = &sortedPixelFreqVec[0];
        
        std::queue<std::shared_ptr<HuffmanTreeNode>> q;
        int node = 0;

        PixelFrequencies pixFreqNode;
        std::shared_ptr<HuffmanTreeNode> root = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(pixFreqs[node]));
        std::shared_ptr<HuffmanTreeNode> currentNode = root;
        
        q.push(root);
        while(node < totalNodes - 1){
            pixFreqNode = pixFreqs[node];
            currentNode = q.front();
            q.pop();
        
            if(currentNode->pix != pixFreqNode.pix){ 
                SwapChildNodes(node, pixFreqNode, pixFreqs);
            }

            if(pixFreqNode.left != nullptr){
                currentNode->left = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixFreqNode.left));
                q.push(currentNode->left);
            }
            if(pixFreqNode.right != nullptr){
                currentNode->right = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixFreqNode.right));
                q.push(currentNode->right);
            }
            node++;
        }

        return root;       

    }
}

#endif