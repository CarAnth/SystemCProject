#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include <systemc.h>
#include <cstdlib>

SC_MODULE(TrafficGenerator) {

    sc_event *ev_NS, *ev_SN, *ev_WE, *ev_EW;

    SC_HAS_PROCESS(TrafficGenerator);
    TrafficGenerator(sc_module_name name,
                     sc_event &ns,
                     sc_event &sn,
                     sc_event &we,
                     sc_event &ew);

    void generate_thread();
};

#endif
