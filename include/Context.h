#ifndef CONTEXT_H
#define CONTEXT_H

#include "Artifact.h"
#include <unordered_map>

class Context{
    public:
        enum BuildArtifact{
            PIXELFREQUENCIES,
            HUFFMANTABLE,
            HUFFMANTREE,
            BMPIMAGE
        };
        virtual void Build() = 0;
        virtual void AddArtifact(BuildArtifact name, Artifact* artifact){
            if(_artifacts.find(name) == _artifacts.end()){
                std::cout << name << " " << artifact << std::endl;
                _artifacts[name] = artifact;
            }
            else{
                DeleteArtifact(name);
                _artifacts[name] = artifact;
            }
        }
        virtual void DeleteArtifact(BuildArtifact name){
            int res = _artifacts.erase(name); //Will call Artifact's destructor
            if(res){
                std::cout << "Artifact did not exist";
            }
        }
        virtual Artifact* GetArtifact(BuildArtifact name){
            if(_artifacts.find(name) == _artifacts.end()){
                return nullptr;
            }
            std::cout << name << " " << &_artifacts[name] << std::endl;
            return _artifacts[name];
        }
    protected:
        Context(){ }
        virtual ~Context(){ 
            for(auto kv : _artifacts){
                delete kv.second;
            }
        }
        std::unordered_map<BuildArtifact, Artifact*> _artifacts;
        const char* contextFilePath;        
};

#endif