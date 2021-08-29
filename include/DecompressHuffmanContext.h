#ifndef DECOMPRESSHUFFMANCONTEXT_H
#define DECOMPRESSHUFFMANCONTEXT_H

#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include <unordered_map>

#include "Context.h"

class DecompressHuffmanContext : Context{
    protected:
         struct HuffTable{
            std::unordered_map<std::string, int> table;
            std::vector<unsigned short int> codeLengths;
            std::vector<std::string> codes;
            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( CEREAL_NVP(table), CEREAL_NVP(codeLengths), CEREAL_NVP(codes) );
            }

            HuffTable(){

            }

            HuffTable(const HuffTable &ht){
                table = ht.table;
                codeLengths = ht.codeLengths;
                codes = ht.codes;
            }


        };

};

#endif