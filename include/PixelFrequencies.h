#ifndef PIXELFREQUENCIES_H
#define PIXELFREQUENCIES_H

class PixelFrequencies : public Artifact{
    public:
        PixelFrequencies(){
            type = Artifact::ArtifactType::PIXELFREQUENCIES;
        }
        PixelFrequencies(int maxCodeLength){
            code = new char[maxCodeLength];
            type = Artifact::ArtifactType::PIXELFREQUENCIES;
        }
        int pix;
        float freq;
        char* code;
        PixelFrequencies *left, *right;  
};

#endif