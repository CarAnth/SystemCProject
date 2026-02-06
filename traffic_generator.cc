#include "traffic_generator.h"
#include <cstdlib>

TrafficGenerator::TrafficGenerator(sc_module_name name,
                                   sc_event &ns,
                                   sc_event &sn,
                                   sc_event &we,
                                   sc_event &ew,
			 	   int mode_)
: sc_module(name), mode(mode_)
{
    ev_NS = &ns;
    ev_SN = &sn;
    ev_WE = &we;
    ev_EW = &ew;

    SC_THREAD(generate_thread);
}

void TrafficGenerator::notify_dir(int dir)
{
    switch (dir)//notifyin sc_event
    {
        case 0: ev_NS->notify(); break;
        case 1: ev_SN->notify(); break;
        case 2: ev_WE->notify(); break;
        case 3: ev_EW->notify(); break;
        deafult: break;
    }
}


void TrafficGenerator::generate_thread()
{
    while (true) {
        if (mode == 0)//random mode
{
    // Random inter-arrival: 1..4 seconds
    wait((rand() % 4) + 1, SC_SEC);

    // This helps demonstrate independent operation: opposite directions can both become green.
    int p = rand() % 100; // 0..99

    if (p < 20) {
        // 20%: generate BOTH directions on an axis at the same simulation time (same delta)
        if (rand() % 2 == 0) {
            // NS axis: NS + SN together
            ev_NS->notify();
            ev_SN->notify();
        } else {
            // WE axis: WE + EW together
            ev_WE->notify();
            ev_EW->notify();
        }
    } else {
        // 80%: single-direction arrival
        int dir = rand() % 4; // 0..3
        notify_dir(dir);
    }
}
else
{
    // targeted
    for (int i = 0; i < 3; i++) { wait(1, SC_SEC); notify_dir(1); } // SN
    for (int i = 0; i < 3; i++) { wait(1, SC_SEC); notify_dir(3); } // EW
    for (int i = 0; i < 3; i++) { wait(1, SC_SEC); notify_dir(0); } // NS
    for (int i = 0; i < 3; i++) { wait(1, SC_SEC); notify_dir(2); } // WE
}
    }
}
