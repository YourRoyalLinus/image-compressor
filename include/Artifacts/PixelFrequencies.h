#ifndef PIXELFREQUENCIES_H
#define PIXELFREQUENCIES_H

#include <memory>

struct PixelFrequencies{
    public:
        PixelFrequencies(){
        }
        
        PixelFrequencies(const PixelFrequencies&) = default;
        
        PixelFrequencies& operator=(const PixelFrequencies&) = default;
        
        int pix;
        float freq;
        std::string code;
        std::shared_ptr<PixelFrequencies> left, right;  
};

#endif