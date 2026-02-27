#include <systemc.h>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "traffic_light_controller.h"
#include "traffic_generator.h"
#include "monitor.h"

int sc_main(int argc, char **argv)
{
    assert(argc==3);

    int sim_s = atoi(argv[1]);
    int mode = atoi(argv[2]);
    
    srand(time(NULL));//random seed for rand()


    sc_time sim_time(sim_s, SC_SEC);

    sc_signal<bool> NS, SN, WE, EW;

	LightController ctrl("Controller");
    ctrl(NS, SN, WE, EW);

    TrafficGenerator gen("Generator",
                         ctrl.ev_NS,
                         ctrl.ev_SN,
                         ctrl.ev_WE,
                         ctrl.ev_EW,
                         mode);

    Monitor mon("Monitor");
    mon(NS, SN, WE, EW);

    sc_start(sc_time(sim_s, SC_SEC));
    return 0;
}
