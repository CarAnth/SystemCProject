#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>
//Module named LightController
SC_MODULE(LightController) {

    // Light outputs
    sc_out<int> NS, SN, WE, EW;//0=red,1=green

    // vehicle arrival events(events)
    sc_event ev_NS, ev_SN, ev_WE, ev_EW;

    bool req_NS, req_SN, req_WE, req_EW;//request flags, for the memory


    SC_HAS_PROCESS(LightController);//module has process
    LightController(sc_module_name name);

    void control_logic();//defining control logic function
};

#endif
