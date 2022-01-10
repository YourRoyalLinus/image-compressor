#ifndef HUFFMANENCODINGCONTEXT_H
#define HUFFMANENCODINGCONTEXT_H

#include "../EncodingContext.h"
#include "../../Artifacts//Huffman/HuffmanTreeNode.h"
#include "../../../include/BinaryHandling/BinaryWriter.h"
#include <vector>
#include <string>

class HuffmanEncodingContext : public EncodingContext{
    public:
        HuffmanEncodingContext(const char* filePath);
        HuffmanEncodingContext(File& f);
        void Build() override;
        void Encode(File& currentFile) override;
        void Decode(File& currentFile) override;
    private:
        std::vector<std::shared_ptr<PixelFrequencies>> pixelFreqs;
        std::shared_ptr<HuffmanTreeNode> rootNode;

        void BuildHuffmanContext();
    
};

#endif