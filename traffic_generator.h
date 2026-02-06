#ifndef TRAFFIC_GENERATOR_H
#define TRAFFIC_GENERATOR_H

#include <systemc.h>
#include <cstdlib>

SC_MODULE(TrafficGenerator) {

    sc_event *ev_NS, *ev_SN, *ev_WE, *ev_EW;
    
    //0 = random, 1 = targeted
    int mode;

    SC_HAS_PROCESS(TrafficGenerator);//name , references, mode_
    TrafficGenerator(sc_module_name name,
                     sc_event &ns,//these uses actual events from controller
                     sc_event &sn,
                     sc_event &we,
                     sc_event &ew,
		     int mode_);

    void generate_thread();

private://we're gonna use only in the generator
    void notify_dir(int dir);

};

#endif
