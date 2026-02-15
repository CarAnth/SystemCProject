#include "traffic_light_controller.h"

LightController::LightController(sc_module_name name)
: sc_module(name)//base class
  {
    SC_METHOD(event_counter);
    dont_initialize();
    sensitive << ev_NS << ev_SN << ev_WE << ev_EW;
    
    SC_THREAD(control_logic);//we will use wait()

    //reset all memories be
    flg_NS = 0;
    flg_SN = 0;
    flg_WE = 0;
    flg_EW = 0; 
  
    //all lights red
    NS.initialize(0);
    SN.initialize(0);
    WE.initialize(0);
    EW.initialize(0);
}

void LightController::event_counter()
{
    bool any = false;
    
    if (ev_NS.triggered()){
      flg_NS++;
      any = true;
    }
    if (ev_SN.triggered()){
      flg_SN++;
      any = true;
    }
    if (ev_WE.triggered()){
      flg_WE++;
      any = true;
    }
    if (ev_EW.triggered()){
      flg_EW++;
      any = true;
    }
    if (any)
        ev_any_req.notify(SC_ZERO_TIME);
}

}

void LightController::control_logic()
{
    //inf while
    while (true) {
        if ((flg_NS + flg_SN + flg_WE + flg_EW) == 0)
            wait(ev_any_req);

        bool ns_memory = (flg_SN + flg_NS) > 0;
        bool we_memory = (flg_WE + flg_EW) > 0;
        
         if (ns_memory && we_memory)
        {
            if (last_was_ns)
                ns_memory = false;
            else
                we_memory = false;
        }
        
        // if NS/SN axis triggered
        if (ns_memory)
        {
            bool serve_ns;
            bool serve_sn;
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
