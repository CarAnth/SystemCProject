#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>
//Module named LightController
SC_MODULE(LightController) {

    // Light outputs
    sc_out<bool> NS, SN, WE, EW;//false=red,true=green

    // vehicle arrival events(events)
    sc_event ev_NS, ev_SN, ev_WE, ev_EW, ev_req;

    int cnt_NS, cnt_SN, cnt_WE, cnt_EW;//request flags, for the memory
    bool last_was_ns;

    SC_HAS_PROCESS(LightController);
    LightController(sc_module_name name);

    void event_counter();
    void control_logic();//defining control logic function
    
    
};

#endif
