#ifndef HUFFMANTREENODE_H
#define HUFFMANTREENODE_H

#include "../Artifact.h"
#include "../PixelFrequencies.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"
#include "memory"

struct HuffmanTreeNode : public Artifact{
    public:
        HuffmanTreeNode(){
            type = Artifact::ArtifactType::HUFFMANTREENODE;
        }

        HuffmanTreeNode(PixelFrequencies& pixelFreq){
            type = Artifact::ArtifactType::HUFFMANTREENODE;
            pix = pixelFreq.pix;
            freq = pixelFreq.freq;
            code = pixelFreq.code;
            left = nullptr;
            right = nullptr;
        }

        HuffmanTreeNode(const HuffmanTreeNode& node){
            type = node.type;
            pix = node.pix;
            freq = node.freq;
            code = node.code;
            left = node.left;
            right = node.right;
        }

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( CEREAL_NVP(pix), CEREAL_NVP(freq), CEREAL_NVP(code), CEREAL_NVP(left), CEREAL_NVP(right) ); // serialize things by passing them to the archive
        }

        int pix;
        int totalNodes;
        float freq;
        std::string code;
        std::shared_ptr<HuffmanTreeNode> left, right; 
};

CEREAL_REGISTER_TYPE(HuffmanTreeNode);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Artifact, HuffmanTreeNode);

#endif