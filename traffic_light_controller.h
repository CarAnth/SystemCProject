#ifndef TRAFFIC_LIGHT_CONTROLLER_H
#define TRAFFIC_LIGHT_CONTROLLER_H

#include <systemc.h>

SC_MODULE(TrafficLightController) {

    // Light outputs
    sc_out<int> NS, SN, WE, EW;

    // Vehicle arrival events
    sc_event ev_NS, ev_SN, ev_WE, ev_EW;

    SC_HAS_PROCESS(TrafficLightController);
    TrafficLightController(sc_module_name name);

    void control_thread();
};

#endif
