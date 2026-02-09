#include "traffic_light_controller.h"

LightController::LightController(sc_module_name name)
: sc_module(name),//base class
  req_NS(false), req_SN(false), req_WE(false), req_EW(false)//all requests initialize to false
{
    SC_THREAD(control_logic);//we will use wait()

    //all lights red
    NS.initialize(0);
    SN.initialize(0);
    WE.initialize(0);
    EW.initialize(0);
}

void LightController::control_logic()
{
    //inf while
    while (true) {
        //wait until at least one vehicle arrives
        wait(ev_NS | ev_SN | ev_WE | ev_EW);

        // req holds memory. we're going to use for if statements
        if (ev_NS.triggered()) req_NS = true;
        if (ev_SN.triggered()) req_SN = true;
        if (ev_WE.triggered()) req_WE = true;
        if (ev_EW.triggered()) req_EW = true;

        bool ns_memory = req_NS || req_SN; 
        bool we_memory = req_WE || req_EW;
          
        
        // if NS/SN axis triggered
        if (ns_memory)
        {
            // cross axis must be red (safety)
            WE->write(0);//WE light goes 0
            EW->write(0);//EW light goes 0

            // independent operation within the axis
            NS->write(req_NS ? 1 : 0); // condition ? value_if_true : value_if_false
            SN->write(req_SN ? 1 : 0);
            
            wait(5, SC_SEC);//wait for 5 sec
            //light goes green for 5 seconds
          
          
            req_NS = false;
            req_SN = false;
            NS->write(0);
            SN->write(0);
            //requests and light resets
        }

        // if WE/EW axis triggered
        if (we_memory)
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
