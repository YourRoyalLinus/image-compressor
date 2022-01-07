#ifndef HUFFMANDECODINGSTRATEGY_H
#define HUFFMANDECODINGSTRATEGY_H

#include "../DecodingStrategy.h"
#include "../../ImageHandling/ImageObjects/JCIFImage.h"
#include "../../Artifacts/Huffman/HuffmanTreeNode.h"
#include "../../../include/BinaryHandling/BinaryReader.h"
#include <vector>

class HuffmanDecodingStrategy : public DecodingStrategy{
    public:
        void Decode(File& currentFile) override;
};

#endif