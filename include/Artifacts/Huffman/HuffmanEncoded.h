#ifndef HUFFMANENCODED_H
#define HUFFMANENCODED_H

#include "../Artifact.h"
#include "./HuffmanTreeNode.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/polymorphic.hpp"

struct HuffmanEncoded : public Artifact{
    public:
        HuffmanEncoded(){
           type = Artifact::ArtifactType::HUFFMANENCODED; 
        }

        HuffmanEncoded(std::shared_ptr<HuffmanTreeNode> r, int e){
            rootNode = r;
            encodedPixelArraySize = e;
            type = Artifact::ArtifactType::HUFFMANENCODED;
        }

        HuffmanEncoded(const HuffmanEncoded& huffEncoded){
            rootNode = huffEncoded.rootNode;
            encodedPixelArraySize = huffEncoded.encodedPixelArraySize;
            type = huffEncoded.type;
        }

        std::shared_ptr<HuffmanTreeNode> rootNode; 
        int encodedPixelArraySize;

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( CEREAL_NVP(rootNode), CEREAL_NVP(encodedPixelArraySize) ); // serialize things by passing them to the archive
        }
};

CEREAL_REGISTER_TYPE(HuffmanEncoded);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Artifact, HuffmanEncoded);

#endif