#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>
//Module named LightController
SC_MODULE(LightController) {

    // Light outputs
    sc_out<int> NS, SN, WE, EW;//0=red,1=green

    // vehicle arrival events(events)
    //Reference: https://www.asic-world.com/systemc/systemc_time4.html#sc_event
    sc_event ev_NS, ev_SN, ev_WE, ev_EW;// we're using sc_event rather than sc_signal<bool> because it's easier to use.

    bool req_NS, req_SN, req_WE, req_EW;//request flags, for the latch

    SC_HAS_PROCESS(LightController);//module has process
    LightController(sc_module_name name);

    void control_thread();//void name
};

#endif
