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

            if(pixelFreq.left != nullptr){
                left = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixelFreq.left));
            }

            if(pixelFreq.right != nullptr){
                right = std::shared_ptr<HuffmanTreeNode>(new HuffmanTreeNode(*pixelFreq.right));
            }
            
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