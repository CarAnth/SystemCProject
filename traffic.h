#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <systemc.h>

// Light states (channel values)
enum Light { RED = 0, GREEN = 1 };

// Directions
enum Dir { NS = 0, SN = 1, WE = 2, EW = 3 };


SC_MODULE(TrafficController)
{
    // Car sensors (channels)
    sc_in<bool> car_ns, car_sn, car_we, car_ew;

    // Light outputs (channels)
    sc_out<Light> light_ns, light_sn, light_we, light_ew;

    sc_time slice;      // decision period
    sc_time green_time; // how long a group stays green before switching

    SC_HAS_PROCESS(TrafficController);
    TrafficController(sc_module_name name,
                      sc_time slice_time = sc_time(100, SC_MS),
                      sc_time green_hold = sc_time(2, SC_SEC));

    void controller_thread();

private:
    bool vert_turn; // toggles between vertical and horizontal to ensure fairness
    bool car(Dir d) const;
    void set_lights(Light ns, Light sn, Light we, Light ew);
};

SC_MODULE(TrafficGenerator)
{
    sc_out<bool> car_ns, car_sn, car_we, car_ew;

    int mode;
    sc_time tick;

    SC_HAS_PROCESS(TrafficGenerator);
    TrafficGenerator(sc_module_name name, int mode_sel, sc_time tick_time = sc_time(1, SC_SEC));

    void gen_thread();

private:
    unsigned rng;
    unsigned next_u();
    bool rand_percent(int p);
    void set_cars(bool ns, bool sn, bool we, bool ew);
};

SC_MODULE(Monitor)
{
    // sensors
    sc_in<bool> car_ns, car_sn, car_we, car_ew;

    // lights
    sc_in<Light> light_ns, light_sn, light_we, light_ew;

    SC_HAS_PROCESS(Monitor);
    Monitor(sc_module_name name);

    void safety_check_method();
    void progress_thread();

private:
    bool waiting[4];
    sc_time since[4];

    bool car(Dir d) const;
    Light light(Dir d) const;
};

#endif // TRAFFIC_H

