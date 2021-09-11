#ifndef HUFFMANTABLE_H
#define HUFFMANTABLE_H

#include "./Artifact.h"
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/polymorphic.hpp"
#include <unordered_map>

//TODO OPTIMIZE
struct HuffmanTable : public Artifact{
    public:
        HuffmanTable(){ 
            type = Artifact::ArtifactType::HUFFMANTABLE;
        }

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

CEREAL_REGISTER_TYPE(HuffmanTable);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Artifact, HuffmanTable);

#endif