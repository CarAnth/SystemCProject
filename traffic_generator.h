#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include <systemc.h>
#include <random>
#include <cstdlib>

SC_MODULE(TrafficGenerator) {

    sc_event *ev_NS, *ev_SN, *ev_WE, *ev_EW;//pointers for events

    SC_HAS_PROCESS(TrafficGenerator);//TrafficGenerator module has process

    TrafficGenerator(sc_module_name name,
                     sc_event &ns,//these uses actual events from controller
                     sc_event &sn,
                     sc_event &we,
                     sc_event &ew);

    //Independent traffic sources for  per direction
    void generate_NS();
    void generate_SN();
    void generate_WE();
    void generate_EW();

    static int next_delay_sec();

};

#endif
