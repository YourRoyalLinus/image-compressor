#ifndef HUFFMANENCODINGCONTEXT_H
#define HUFFMANENCODINGCONTEXT_H

#include "ContextBuilderHelper.h"
#include "EncodingContext.h"
#include "HuffmanTree.h"
#include "HuffmanTable.h"

class HuffmanEncodingContext : public EncodingContext{
    public:
        HuffmanEncodingContext(const char* filePath);
        ~HuffmanEncodingContext();
        void Build() override;
        HuffmanTree* GetHuffmanTree();
        HuffmanTable* GetHuffmanTable();
    private:
        void BuildHuffmanContext();
    
};

#endif