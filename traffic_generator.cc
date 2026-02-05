#include "traffic_generator.h"

TrafficGenerator::TrafficGenerator(sc_module_name name,
                                   sc_event &ns,
                                   sc_event &sn,
                                   sc_event &we,
                                   sc_event &ew)
: sc_module(name)
{
    ev_NS = &ns;
    ev_SN = &sn;
    ev_WE = &we;
    ev_EW = &ew;

    SC_THREAD(generate_thread);
}

void TrafficGenerator::generate_thread()
{
    while (true) {
        wait(rand() % 5 + 1, SC_SEC);

        int r = rand() % 4;
        switch (r) {
            case 0: ev_NS->notify(); break;
            case 1: ev_SN->notify(); break;
            case 2: ev_WE->notify(); break;
            case 3: ev_EW->notify(); break;
        }
    }
}
