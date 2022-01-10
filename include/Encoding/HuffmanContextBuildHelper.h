#ifndef HUFFMANCONTEXTBUILDHELPER_H
#define HUFFMANCONTEXTBUILDHELPER_H

#include "../Artifacts/PixelFrequencies.h"
#include "../Artifacts/Huffman/HuffmanTreeNode.h"
#include "./ContextBuildHelper.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <queue>
#include <algorithm>


namespace ContextBuilder{
    namespace {
        bool ComparePixFreqsGreaterFrequencies(const std::shared_ptr<PixelFrequencies>& lhs, const std::shared_ptr<PixelFrequencies>& rhs){
            return lhs->freq > rhs->freq;
        }

        bool ComparePixFreqsLesserFrequencies(const std::shared_ptr<PixelFrequencies>& lhs, const std::shared_ptr<PixelFrequencies>& rhs){
            return lhs->freq < rhs->freq;
        }
    }

    std::vector<std::shared_ptr<PixelFrequencies>> InitializePixelFrequencies(){ 
        return std::vector<std::shared_ptr<PixelFrequencies>>();     
    }


    void InitializeLeafNodes(const std::vector<int>& hist, int totalPixels, std::vector<std::shared_ptr<PixelFrequencies>>& pixFreqs){
        float tempProb;

        for(unsigned i = 0; i < 256; i++){
            if(hist[i] != 0){
                std::shared_ptr<PixelFrequencies> tmpFreq = std::shared_ptr<PixelFrequencies>(new PixelFrequencies());

                tmpFreq->pix = i;

                tempProb = (float)hist[i] / (float)totalPixels; 
                tmpFreq->freq = tempProb;
                
                tmpFreq->left = nullptr; 
                tmpFreq->right = nullptr;
                
                pixFreqs.push_back(tmpFreq);
            }
        }
    }
    
    void SortPixFreqsAscending(std::vector<std::shared_ptr<PixelFrequencies>>& pixFreqs){
        std::sort(pixFreqs.begin(), pixFreqs.end(), ComparePixFreqsLesserFrequencies);
    }

    void CreateHuffmanTree(int nonZeroNodes, std::vector<std::shared_ptr<PixelFrequencies>>& pfs){
        float sumProb;
        int sumPix;
        int treePosition = 0;
        int n = 0;

        std::vector<std::shared_ptr<PixelFrequencies>> sortedPF(pfs);

        while (n < nonZeroNodes - 1) 
        {

            sumProb = sortedPF[treePosition]->freq + sortedPF[treePosition+1]->freq;
            sumPix = sortedPF[treePosition]->pix + sortedPF[treePosition+1]->pix;

            std::shared_ptr<PixelFrequencies> nextFreq = std::shared_ptr<PixelFrequencies>(new PixelFrequencies());
            nextFreq->pix = sumPix;
            nextFreq->freq = sumProb;
            nextFreq->left = sortedPF[treePosition];
            nextFreq->right = sortedPF[treePosition+1];

            pfs.push_back(nextFreq);
            sortedPF.push_back(nextFreq);
            sortedPF.erase(sortedPF.begin(), sortedPF.begin()+2);

            SortPixFreqsAscending(sortedPF);

            n++;
        }
    }

    void ConcatCodes(std::string& childCode, const std::string& parentCode, char add){
        if(childCode.size() < parentCode.size()){
            childCode.resize(parentCode.size());
        }

        for(std::size_t i = 0; i < parentCode.size(); i++){
            childCode[i] = parentCode[i];
        }

        childCode += add;
    }
    
    void Backtrack(int nodes, int totalNodes, std::vector<std::shared_ptr<PixelFrequencies>>& pixFreqs){
        const char left = '0';
        const char right = '1';
        
        for(int i = totalNodes - 1; i >= nodes; i--){
            if(pixFreqs[i]->left != nullptr){
                ConcatCodes(pixFreqs[i]->left->code, pixFreqs[i]->code, left);
            }
            if(pixFreqs[i]->right != nullptr){
                ConcatCodes(pixFreqs[i]->right->code, pixFreqs[i]->code, right);
            }
        }
    }

    void SortPixelFreqsDescending(std::vector<std::shared_ptr<PixelFrequencies>>& pfs){
        std::sort(pfs.begin(), pfs.end(), ComparePixFreqsGreaterFrequencies);
    }    
}

#endif