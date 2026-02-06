#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>

SC_MODULE(TrafficLightController) {

    // Light outputs
    sc_out<int> NS, SN, WE, EW;//0=red,1=green

    // vehicle arrival events(events)
    sc_event ev_NS, ev_SN, ev_WE, ev_EW;

    bool req_NS, req_SN, req_WE, req_EW;//request flags, for the latch

    SC_HAS_PROCESS(TrafficLightController);//module has process
    TrafficLightController(sc_module_name name);

    void control_thread();//void name
};

#endif
