#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"
#include <unordered_map>

class Huffman{
    public:
        struct HuffmanTree{
            public:
            int pix;
            int arrloc;
            float freq;
        };
        struct HuffmanTable{
            HuffmanTable(){ }

            HuffmanTable(const HuffmanTable &ht){
                table = ht.table;
                codeLengths = ht.codeLengths;
                codes = ht.codes;
            }

            std::unordered_map<std::string, int> table;
            std::vector<unsigned short int> codeLengths;
            std::vector<std::string> codes;

            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( CEREAL_NVP(table), CEREAL_NVP(codeLengths), CEREAL_NVP(codes) ); // serialize things by passing them to the archive
            }
        };

        Huffman(HuffmanTree* tree, HuffmanTable* table){
            huffTree = tree;
            huffTable = table;
        }

        Huffman(){
        }

        HuffmanTree* GetHuffmanTree(){
            return huffTree;
        }

        HuffmanTable* GetHuffmanTable(){
            return huffTable;
        }

        protected:
            HuffmanTree* huffTree = 0;
            HuffmanTable* huffTable = 0;
};

#endif