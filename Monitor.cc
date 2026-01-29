#include "traffic.h"
#include <cassert>
#include <iostream>

Monitor::Monitor(sc_module_name name)
: sc_module(name)
{
    for (int i = 0; i < 4; i++) {
        waiting[i] = false;
        since[i] = SC_ZERO_TIME;
    }

    // Safety check whenever any light changes
    SC_METHOD(safety_check_method);
    dont_initialize();
    sensitive << light_ns << light_sn << light_we << light_ew;

    // Progress / eventual green demonstration (needs time => thread)
    SC_THREAD(progress_thread);
}

bool Monitor::car(Dir d) const
{
    switch (d) {
        case NS: return car_ns.read();
        case SN: return car_sn.read();
        case WE: return car_we.read();
        case EW: return car_ew.read();
        default: return false;
    }
}

Light Monitor::light(Dir d) const
{
    switch (d) {
        case NS: return light_ns.read();
        case SN: return light_sn.read();
        case WE: return light_we.read();
        case EW: return light_ew.read();
        default: return RED;
    }
}

void Monitor::safety_check_method()
{
    // Safety requirement example: SN must not be green at same time as WE is green
    // (More generally: vertical and horizontal must not be green simultaneously.)
    bool vertical_green = (light(NS) == GREEN) || (light(SN) == GREEN);
    bool horizontal_green = (light(WE) == GREEN) || (light(EW) == GREEN);

    assert(!(vertical_green && horizontal_green));
}

void Monitor::progress_thread()
{
    // Demonstrate "eventually green": record arrival time, print time-to-green
    for (;;) {
        for (int i = 0; i < 4; i++) {
            Dir d = (Dir)i;

            if (car(d) && !waiting[i]) {
                waiting[i] = true;
                since[i] = sc_time_stamp();
                std::cout << sc_time_stamp() << " car arrived at dir " << i << "\n";
            }

            if (waiting[i] && (light(d) == GREEN)) {
                sc_time w = sc_time_stamp() - since[i];
                std::cout << sc_time_stamp() << " dir " << i
                          << " got GREEN after " << w.to_seconds() << " seconds\n";
                waiting[i] = false;
            }

            if (!car(d)) {
                waiting[i] = false; // no car -> not waiting
            }
        }

        wait(100, SC_MS);
    }
}

