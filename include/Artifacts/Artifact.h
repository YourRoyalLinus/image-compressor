#ifndef ARTIFACT_H
#define ARTIFACT_H

class Artifact{
    public:
        enum ArtifactType{
            BMPIMAGE,
            PIXELFREQUENCIES,
            HUFFMANTREE,
            HUFFMANTREENODE
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