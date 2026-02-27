#ifndef MONITOR_H
#define MONITOR_H

#include <systemc.h>
#include <cassert>

SC_MODULE(Monitor) {

    sc_in<bool> NS, SN, WE, EW;//inputs

    SC_HAS_PROCESS(Monitor);
    Monitor(sc_module_name name);

    void check_and_print();
};

#endif
