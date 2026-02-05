#include "monitor.h"

Monitor::Monitor(sc_module_name name)
: sc_module(name)
{
    SC_METHOD(safety_check);
    dont_initialize();
    sensitive << NS << SN << WE << EW;
}

void Monitor::safety_check()
{
    // Safety: conflicting greens forbidden
    assert(!(NS.read() == 1 && WE.read() == 1));
    assert(!(SN.read() == 1 && EW.read() == 1));
}
