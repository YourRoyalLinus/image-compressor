#ifndef ARTIFACT_H
#define ARTIFACT_H

class Artifact{
    public:
        Artifact* GetArtifact(){
            return this; 
        }

        void DeleteArtifact(){
            delete this;
        }
        virtual ~Artifact(){ } 
    protected:
        Artifact() { }   
};


#endif