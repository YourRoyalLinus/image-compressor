#ifndef PIXELFREQUENCIES_H
#define PIXELFREQUENCIES_H

#include <memory>
struct PixelFrequencies{
    public:
        PixelFrequencies(){
        }
        PixelFrequencies(int maxCodeLength){
            code = std::shared_ptr<char[]>(new char[maxCodeLength]);
        }
        
        int pix;
        float freq;
        std::shared_ptr<char[]> code;
        PixelFrequencies *left, *right;  
};

#endif