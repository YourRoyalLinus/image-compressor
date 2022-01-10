#ifndef CONTEXTBUILDHELPER_H
#define CONTEXTBUILDHELPER_H

#include "../Utils/GeneralUtility.h"
#include <vector>
#include <algorithm>

namespace ContextBuilder{
    inline std::vector<int> GetHistogram(int pixelBufferSize, unsigned char* pixelDataArray ){
        std::vector<int> hist(256);
        for(unsigned i = 0; i < 256; i++){
            hist[i] = 0;
        }

        for(int i = 0; i < pixelBufferSize; i++){
            int pixIntensityValue = *(pixelDataArray+i);
            hist[pixIntensityValue] += 1;
        }

        return hist;
    }

    inline int GetNonZeroOccurances(std::vector<int>& hist){
        int nodes = 0;
        for(unsigned i = 0; i < 256; i++){
            if(hist[i] != 0){
                nodes++;
            }
        }

        return nodes;
    }

    inline int GetMaxCodeLength(float p){
        int i = 0;
        while((1/p) > Utility::FibonacciSeq(i)){
            i++;
        }

        return (i - 3);
    }

    inline int GetTotalNodes(int nodes){
        return 2 * nodes - 1;
    }

    inline float GetMinimumProbabilityOfOccurance(std::vector<int>& hist, int imageWidth, int imageHeight){
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
}

#endif