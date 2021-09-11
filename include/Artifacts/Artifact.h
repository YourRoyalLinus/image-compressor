#ifndef ARTIFACT_H
#define ARTIFACT_H

class Artifact{
    public:
        enum ArtifactType{
            PIXELFREQUENCIES,
            HUFFMANTABLE,
            HUFFMANTREE,
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