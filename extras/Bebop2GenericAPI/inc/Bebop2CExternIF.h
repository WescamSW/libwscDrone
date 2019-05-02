#ifndef BEBOP2_CEXTERNIF
#define BEBOP2_CEXTERNIF
/* 
    CURRENTY written as the header for a C++ file but
    could be done for a C file as well. Consider either
    naming hpp or destroying later
*/
#include "Bebop2CtrlIF.h"

class Bebop2CExternIF : public Bebop2CtrlIF
{
    public:

        Bebop2FrameIF(int callsign);

        ~Bebop2CExternIF() {}

    private:
        //this would only be called by the constructor
        void init()
};

#endif