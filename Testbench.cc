#include <cassert>
#include <systemc.h>
#include "traffic.h"

int sc_main(int argc, char** argv)
{
    // Command-line arguments (same style as tutorial examples):
    // 1) simulation time (in seconds)
    // 2) traffic mode: 0 = random, 1 = targeted
    assert(argc == 3);

    sc_time sim_time(atof(argv[1]), SC_SEC);
    int mode = atoi(argv[2]);

    // Car Sensors
    sc_signal<bool> car_ns_sig;
    sc_signal<bool> car_sn_sig;
    sc_signal<bool> car_we_sig;
    sc_signal<bool> car_ew_sig;

    // Light Sensors
    sc_signal<Light> light_ns_sig;
    sc_signal<Light> light_sn_sig;
    sc_signal<Light> light_we_sig;
    sc_signal<Light> light_ew_sig;

    TrafficController ctrl("Controller", sc_time(100, SC_MS));
    TrafficGenerator  gen("Generator", mode, sc_time(1, SC_SEC));
    Monitor           mon("Monitor");

    //Sensor channels
    gen(car_ns_sig, car_sn_sig, car_we_sig, car_ew_sig);

    ctrl(car_ns_sig, car_sn_sig, car_we_sig, car_ew_sig,
         light_ns_sig, light_sn_sig, light_we_sig, light_ew_sig);

    mon(car_ns_sig, car_sn_sig, car_we_sig, car_ew_sig,
        light_ns_sig, light_sn_sig, light_we_sig, light_ew_sig);

    sc_start(sim_time);
    return 0;
}
