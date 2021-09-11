#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "../Artifact.h"

struct HuffmanTree : public Artifact{
    public:
        HuffmanTree(){
            type = Artifact::ArtifactType::HUFFMANTREE;
        }
        int pix;
        int arrloc;
        float freq;
};

#endif