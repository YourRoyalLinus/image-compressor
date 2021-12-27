#ifndef HUFFMANTREENODE_H
#define HUFFMANTREENODE_H

#include "../PixelFrequencies.h"
#include "../../../lib/Cereal-1.3.0/include/cereal/archives/binary.hpp"
#include "../../../lib/Cereal-1.3.0/include/cereal/types/string.hpp"
#include "../../../lib/Cereal-1.3.0/include/cereal/types/polymorphic.hpp"
#include "memory"

struct HuffmanTreeNode{
    public:
        HuffmanTreeNode(){
        }

        HuffmanTreeNode(const PixelFrequencies& pixelFreq){
            pix = pixelFreq.pix;
            code = pixelFreq.code;
            left = nullptr;
            right = nullptr;
        }

        HuffmanTreeNode(const HuffmanTreeNode& node){
            pix = node.pix;
            code = node.code;
            left = node.left;
            right = node.right;
        }

        HuffmanTreeNode& operator=(const HuffmanTreeNode& rhs){
            pix = rhs.pix;
            code = rhs.code;
            left = rhs.left;
            right = rhs.right;
        }

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( CEREAL_NVP(pix), CEREAL_NVP(code), CEREAL_NVP(left), CEREAL_NVP(right) );
        }

        int pix;
        std::string code;
        std::shared_ptr<HuffmanTreeNode> left, right; 
};

#endif