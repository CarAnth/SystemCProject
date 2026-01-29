#include <iostream>
#include "traffic.h"

using std::cout;
using std::endl;

TrafficController::TrafficController(sc_module_name name, sc_time slice_time, sc_time green_hold)
: sc_module(name),
  slice(slice_time),
  green_time(green_hold),
  vert_turn(true)
{
    grant_ns.initialize(false);
    grant_sn.initialize(false);
    grant_we.initialize(false);
    grant_ew.initialize(false);

    SC_THREAD(scheduler_thread);
}

bool TrafficController::car(Dir d) const
{
    switch (d) {
        case NS: return car_ns.read();
        case SN: return car_sn.read();
        case WE: return car_we.read();
        case EW: return car_ew.read();
        default: return false;
    }
}

void TrafficController::set_grants(bool ns, bool sn, bool we, bool ew)
{
    grant_ns.write(ns);
    grant_sn.write(sn);
    grant_we.write(we);
    grant_ew.write(ew);
}

void TrafficController::all_red()
{
    set_grants(false, false, false, false);
}

void TrafficController::scheduler_thread()
{

    Group active = VERTICAL;

    for (;;)
    {
        // decide if we should switch groups (if active group has no cars but other has)
        bool vert_has = car(NS) || car(SN);
        bool horiz_has = car(WE) || car(EW);

        if (active == VERTICAL && !vert_has && horiz_has) {
            all_red();
            if (min_gap != SC_ZERO_TIME) wait(min_gap);
            active = HORIZONTAL;
        } else if (active == HORIZONTAL && !horiz_has && vert_has) {
            all_red();
            if (min_gap != SC_ZERO_TIME) wait(min_gap);
            active = VERTICAL;
        }

        // compute grants for the active group only
        bool g_ns=false, g_sn=false, g_we=false, g_ew=false;

        if (active == VERTICAL) {
            if (rr_vert == 0) {
                g_ns = car(NS);
                g_sn = car(SN);
            } else {
                g_sn = car(SN);
                g_ns = car(NS);
            }
            g_we = false; g_ew = false;

            rr_vert ^= 1;
        } else {
            if (rr_horiz == 0) {
                g_we = car(WE);
                g_ew = car(EW);
            } else {
                g_ew = car(EW);
                g_we = car(WE);
            }
            g_ns = false; g_sn = false;

            rr_horiz ^= 1;
        }

        set_grants(g_ns, g_sn, g_we, g_ew);

        cout << sc_time_stamp() << " [CTRL] active_group="
             << ((active==VERTICAL) ? "VERTICAL" : "HORIZONTAL")
             << " grants: NS=" << (g_ns?1:0)
             << " SN=" << (g_sn?1:0)
             << " WE=" << (g_we?1:0)
             << " EW=" << (g_ew?1:0)
             << endl;

        wait(slice);
    }
}
