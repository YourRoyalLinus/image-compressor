#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

struct HuffmanTree{
    public:
        virtual ~HuffmanTree(){ //C++ voodoo-warlock magic requires this to be a virtual destructor or there will be large mem leaks
        }
        int pix;
        int arrloc;
        float freq;
};

#endif