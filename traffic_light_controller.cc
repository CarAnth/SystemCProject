#include "traffic_light_controller.h"

TrafficLightController::TrafficLightController(sc_module_name name)
: sc_module(name),//base class
  req_NS(false), req_SN(false), req_WE(false), req_EW(false)//all requests initialize to false
{
    SC_THREAD(control_thread);//we will use wait()

    //all lights red
    NS.initialize(0);
    SN.initialize(0);
    WE.initialize(0);
    EW.initialize(0);
}

void TrafficLightController::control_thread()
{
    //inf while
    while (true) {

        //wait until at least one vehicle arrives
        wait(ev_NS | ev_SN | ev_WE | ev_EW);

        // latch requests (valid right after wait)
        if (ev_NS.triggered()) req_NS = true;
        if (ev_SN.triggered()) req_SN = true;
        if (ev_WE.triggered()) req_WE = true;
        if (ev_EW.triggered()) req_EW = true;

        // if NS/SN axis triggered
        if (req_NS || req_SN)
        {
            // cross axis must be red (safety)
            WE->write(0);
            EW->write(0);

            // independent operation within the axis
            NS->write(req_NS ? 1 : 0);
            SN->write(req_SN ? 1 : 0);

            wait(5, SC_SEC);

            // clear served requests and go red
            req_NS = false;
            req_SN = false;
            NS->write(0);
            SN->write(0);
        }

        // if WE/EW axis triggered
        if (req_WE || req_EW)
        {
            // cross axis must be red (safety)
            NS->write(0);
            SN->write(0);

            // independent operation within the axis
            WE->write(req_WE ? 1 : 0);
            EW->write(req_EW ? 1 : 0);

            wait(5, SC_SEC);

            // clear served requests and go red
            req_WE = false;
            req_EW = false;
            WE->write(0);
            EW->write(0);
        }
    }
}
