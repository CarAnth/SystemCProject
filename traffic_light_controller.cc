#include "traffic_light_controller.h"

LightController::LightController(sc_module_name name)
: sc_module(name)//base class
  {
    
    SC_METHOD(event_counter);
    dont_initialize();
    sensitive << ev_NS << ev_SN << ev_WE << ev_EW;
    
    SC_THREAD(control_logic);//we will use wait()

    //reset all memories be
    cnt_NS = 0;
    cnt_SN = 0;
    cnt_WE = 0;
    cnt_EW = 0; 
    last_was_ns = 0;
    
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
      cnt_NS++;
      any = true;
    }
    if (ev_SN.triggered()){
      cnt_SN++;
      any = true;
    }
    if (ev_WE.triggered()){
      cnt_WE++;
      any = true;
    }
    if (ev_EW.triggered()){
      cnt_EW++;
      any = true;
    }
    if (any)
        ev_req.notify(SC_ZERO_TIME);
}


void LightController::control_logic()
{
    const int CAP_PER_DIR = 5;
    //inf while
    while (true) {
        if ((cnt_NS + cnt_SN + cnt_WE + cnt_EW) == 0)
            wait(ev_req);

        bool ns_memory = (cnt_SN + cnt_NS) > 0;
        bool we_memory = (cnt_WE + cnt_EW) > 0;
        
         if (ns_memory && we_memory)//fairness logic
        {
            if (last_was_ns)
                ns_memory = false;
            else
                we_memory = false;
        }
        
        // if NS/SN axis triggered
        if (ns_memory)
        {
            bool d_NS = (cnt_NS > 0);
            bool d_SN = (cnt_SN > 0);
            
            // cross axis must be red (safety)
            WE->write(false);//WE light goes false
            EW->write(false);//EW light goes false

            // independent operation within the axis
            NS->write(d_NS ? true : false); // condition ? value_if_true : value_if_false
            SN->write(d_SN ? true : false);
            
            for (int i = 0; i < 5; i++)
            {
              bool d_NS_now =(cnt_NS>0);
              bool d_SN_now =(cnt_SN>0);
              
              WE->write(false);
              EW->write(false);
              
              NS->write(d_NS_now ? true : false);
              SN->write(d_SN_now ? true : false);
              
              bool ns_empty=!(d_SN_now && d_SN_now);
              bool we_active = (cnt_WE + cnt_EW > 0);
              if(ns_empty && we_active){
                break;
              }
              wait(1,SC_SEC);

              if(d_NS && cnt_NS > 0){
                cnt_NS--;
              }
              if(d_SN && cnt_SN > 0){
                cnt_SN--;
              }
            }
            NS->write(false);
            SN->write(false);
            //requests and light resets

            last_was_ns=true;
        }

        // if WE/EW axis triggered
        if (we_memory)
        {
            bool d_WE = (cnt_WE > 0);
            bool d_EW = (cnt_EW > 0);
            // cross axis must be red (safety)
            NS->write(false);
            SN->write(false);

            // independent operation within the axis
            WE->write(d_WE ? true : false);
            EW->write(d_EW ? true : false);

            for (int i = 0; i < 5; i++)
            {
              bool d_WE_now = (cnt_WE > 0);
              bool d_EW_now = (cnt_EW > 0);
              
              NS->write(false);
              SN->write(false);

              WE->write(d_WE_now ? true : false);
              EW->write(d_EW_now ? true : false);

              bool we_empty=!(d_WE_now && d_EW_now);
              bool ns_active = (cnt_NS + cnt_SN > 0);
              if(we_empty && ns_active){
                break;
              }
              wait(1,SC_SEC);

              if(d_WE && cnt_WE > 0){
                cnt_NS--;
              }
              if(d_EW && cnt_EW > 0){
                cnt_SN--;
              }

            }
            WE->write(false);
            EW->write(false);
            last_was_ns=false;
        }
    }
}
