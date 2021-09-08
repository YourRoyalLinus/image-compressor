#ifndef CONTEXT_H
#define CONTEXT_H

#include "Artifact.h"
#include <iostream>
#include <unordered_map>

class Context{
    public:
        virtual void Build() = 0;
        virtual void AddArtifact(Artifact::ArtifactType name, Artifact* artifact){
            if(_artifacts.find(name) == _artifacts.end()){
                _artifacts[name] = artifact;
            }
            else{
                DeleteArtifact(name);
                _artifacts[name] = artifact;
            }
        }
        virtual void DeleteArtifact(Artifact::ArtifactType name){
            int res = _artifacts.erase(name); //Will call Artifact's destructor
            if(res){
                std::cout << "Artifact did not exist";
            }
        }
        virtual Artifact* GetArtifact(Artifact::ArtifactType name){
            if(_artifacts.find(name) == _artifacts.end()){
                return nullptr;
            }
            return _artifacts[name];
        }
    protected:
        Context(){ }
        virtual ~Context(){ 
            for(auto kv : _artifacts){
                delete kv.second;
            }
        }
        std::unordered_map<Artifact::ArtifactType, Artifact*> _artifacts;
        const char* contextFilePath;        
};

#endif