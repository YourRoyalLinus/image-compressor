#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

struct Uncopyable{
    protected:
        Uncopyable(){
        }
        ~Uncopyable(){
        }
    private:
        Uncopyable(const Uncopyable&) = default;
        Uncopyable& operator=(const Uncopyable&) = default;
};

#endif