#ifndef COMMANDLINEARG_H
#define COMMANDLINEARG_H

#include <vector>
#include <string>

class CommandLineArg{
    public:
        struct Artifact{
            std::vector<std::string> files;
        };
        virtual void Handle() = 0;
        virtual void SetFlag() = 0;

        void SetArtifact(Artifact a){
            artifact = a;
        }

        Artifact GetArtifact(){
            return artifact;
        }
;
        char GetFlag(){
            return flag;
        }
    protected:
        char flag;
        Artifact artifact;
};

#endif