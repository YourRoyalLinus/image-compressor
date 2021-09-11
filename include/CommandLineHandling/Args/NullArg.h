#ifndef NULLARG_H
#define NULLARG_H

#include "../../CommandLineHandling/Args/CommandLineArg.h"

class NullArg : public CommandLineArg{
    public:
        NullArg();
        void Handle() override;
        void SetFlag() override;
};

#endif