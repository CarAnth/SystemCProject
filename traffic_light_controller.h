#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>
//Module named LightController
SC_MODULE(LightController) {

    // Light outputs
    sc_out<bool> NS, SN, WE, EW;//false=red,true=green

    // vehicle arrival events(events)
    sc_event ev_NS, ev_SN, ev_WE, ev_EW;

    bool flg_NS, flg_SN, flg_WE, flg_EW;//request flags, for the memory

    SC_HAS_PROCESS(LightController);
    LightController(sc_module_name name);

    void control_logic();//defining control logic function
    
};

#endif
