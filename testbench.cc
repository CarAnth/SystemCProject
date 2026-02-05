#include <systemc.h>
#include "traffic_light_controller.h"
#include "traffic_generator.h"
#include "monitor.h"

int sc_main(int argc, char **argv)
{
    sc_signal<int> NS, SN, WE, EW;

    TrafficLightController ctrl("Controller");
    ctrl(NS, SN, WE, EW);

    TrafficGenerator gen("Generator",
                         ctrl.ev_NS,
                         ctrl.ev_SN,
                         ctrl.ev_WE,
                         ctrl.ev_EW);

    Monitor mon("Monitor");
    mon(NS, SN, WE, EW);

    sc_start(60, SC_SEC);
    return 0;
}
