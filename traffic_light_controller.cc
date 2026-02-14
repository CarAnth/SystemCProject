#include "traffic_light_controller.h"

LightController::LightController(sc_module_name name)
: sc_module(name)//base class
  {
    SC_THREAD(control_logic);//we will use wait()
    //reset all memories 
    flg_NS = false;
    flg_SN = false;
    flg_WE = false;
    flg_EW = false; 
  
    //all lights red
    NS.initialize(0);
    SN.initialize(0);
    WE.initialize(0);
    EW.initialize(0);
}

void LightController::event_request()
{

}

void LightController::control_logic()
{
    //inf while
    while (true) {
        //wait until at least one vehicle arrives
        wait(ev_NS | ev_SN | ev_WE | ev_EW); // |=sc_event_or_list

        // req holds memory. we're going to use for if statements
        if (ev_NS.triggered()) flg_NS = true;
        if (ev_SN.triggered()) flg_SN = true;
        if (ev_WE.triggered()) flg_WE = true;
        if (ev_EW.triggered()) flg_EW = true;

        bool ns_memory = flg_NS || flg_SN; 
        bool we_memory = flg_WE || flg_EW;
          
        
        // if NS/SN axis triggered
        if (ns_memory)
        {
            // cross axis must be red (safety)
            WE->write(false);//WE light goes false
            EW->write(false);//EW light goes false

            // independent operation within the axis
            NS->write(flg_NS ? true : false); // condition ? value_if_true : value_if_false
            SN->write(flg_SN ? true : false);
            
            wait(5, SC_SEC);//wait for 5 sec
            //light goes green for 5 seconds
          
            flg_NS = false;
            flg_SN = false;
            NS->write(false);
            SN->write(false);
            //requests and light resets
        }

        // if WE/EW axis triggered
        if (we_memory)
        {
            // cross axis must be red (safety)
            NS->write(false);
            SN->write(false);

            // independent operation within the axis
            WE->write(flg_WE ? true : false);
            EW->write(flg_EW ? true : false);

            wait(5, SC_SEC);

            // clear served requests and go red
            flg_WE = false;
            flg_EW = false;
            WE->write(false);
            EW->write(false);
        }
    }
}
