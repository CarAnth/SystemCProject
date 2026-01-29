// TrafficGenerator.cpp
#include <iostream>
#include "traffic.h"

using std::cout;
using std::endl;

TrafficGenerator::TrafficGenerator(sc_module_name name, int mode_sel, sc_time tick_time)
: sc_module(name),
  mode(mode_sel),
  tick(tick_time),
  rng(1234567u)
{
    car_ns.initialize(false);
    car_sn.initialize(false);
    car_we.initialize(false);
    car_ew.initialize(false);

    SC_THREAD(gen_thread);
}

unsigned TrafficGenerator::next_u32()
{
    rng = rng * 1664525u + 1013904223u;
    return rng;
}

bool TrafficGenerator::rand_bool_percent(int p)
{
    if (p <= 0) return false;
    if (p >= 100) return true;
    return (next_u32() % 100u) < (uint32_t)p;
}

void TrafficGenerator::set_all(bool ns, bool sn, bool we, bool ew)
{
    car_ns.write(ns);
    car_sn.write(sn);
    car_we.write(we);
    car_ew.write(ew);
}

void TrafficGenerator::gen_thread()
{
    if (mode == 0) {

        for (;;) {
            set_cars(rand_percent(35),
	   	     rand_percent(25),
		     rand_percent(30),
		     rand_percent(20));
            wait(tick);
        }
    } else {
        for (;;) {
            // Phase 1: conflicting arrivals (VERTICAL + HORIZONTAL both have cars)
            set_all(true,  false, true,  false); // NS + WE waiting
            cout << sc_time_stamp() << " [GEN-TGT] Phase1: NS & WE waiting (conflict test)" << endl;
            wait(5, SC_SEC);

            // Phase 2: keep SN waiting long (fairness)
            set_all(false, true,  false, false); // only SN
            cout << sc_time_stamp() << " [GEN-TGT] Phase2: SN waiting (eventually green test)" << endl;
            wait(10, SC_SEC);

            // Phase 3: horizontal burst
            set_all(false, false, true, true);   // WE + EW
            cout << sc_time_stamp() << " [GEN-TGT] Phase3: WE & EW waiting (independence within group)" << endl;
            wait(8, SC_SEC);

            // Phase 4: all quiet
            set_all(false, false, false, false);
            cout << sc_time
        }
    }
}
