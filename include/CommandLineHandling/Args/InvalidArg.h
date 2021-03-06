#ifndef INVALIDARG_H
#define INVALIDARG_H

#include "../../CommandLineHandling/Args/CommandLineArg.h"

class InvalidArg : public CommandLineArg{
    public:
        InvalidArg(char arg);
        void Handle() override;
        void SetFlag() override;
    private:
        std::string optArg;
};

#endif