#ifndef CONTEXT_H
#define CONTEXT_H

class Context{
    public:
        virtual void Build() = 0;
        virtual ~Context(){ }
    protected:
        Context(){ }      
        const char* contextFilePath;        
};

#endif