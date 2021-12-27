#ifndef PIXELFREQUENCIES_H
#define PIXELFREQUENCIES_H

struct PixelFrequencies{
    public:
        PixelFrequencies(){
        }
        PixelFrequencies(int maxCodeLength){
            code = new char[maxCodeLength];
        }
        
        int pix;
        float freq;
        char* code;
        PixelFrequencies *left, *right;  
};

#endif