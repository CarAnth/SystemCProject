#include "traffic_light_controller.h"

TrafficLightController::TrafficLightController(sc_module_name name)
: sc_module(name)
{
    SC_THREAD(control_thread);

    NS.initialize(0);
    SN.initialize(0);
    WE.initialize(0);
    EW.initialize(0);
}

void TrafficLightController::control_thread()
{
    while (true) {

        // Wait for any vehicle
        wait(ev_NS | ev_SN | ev_WE | ev_EW);

        // NS / SN phase
        if (ev_NS.triggered() || ev_SN.triggered()) {
            NS->write(1);
            SN->write(0);
            WE->write(0);
            EW->write(0);
            wait(5, SC_SEC);
            NS->write(0);
        }

        // WE / EW phase
        if (ev_WE.triggered() || ev_EW.triggered()) {
            WE->write(1);
            EW->write(0);
            NS->write(0);
            SN->write(0);
            wait(5, SC_SEC);
            WE->write(0);
        }
    }
}
