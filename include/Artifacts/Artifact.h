#ifndef ARTIFACT_H
#define ARTIFACT_H

class Artifact{
    public:
        enum ArtifactType{
            PIXELFREQUENCIES,
            HUFFMANENCODED,
            HUFFMANTABLE,
            HUFFMANTREE,
            HUFFMANTREENODE,
            BMPIMAGE
        };
        
        ArtifactType GetArtifactType(){
            return type; 
        }

        void DeleteArtifact(){
            delete this;
        }

        virtual ~Artifact(){ } 
    protected:
        Artifact() { } 
        ArtifactType type;  
};


#endif