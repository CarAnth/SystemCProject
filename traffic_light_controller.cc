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
    NS.initialize(false);
    SN.initialize(false);
    WE.initialize(false);
    EW.initialize(false);
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
            int serve_NS = (cnt_NS < 5) ? cnt_NS : 5;
            int serve_SN = (cnt_SN < 5) ? cnt_SN : 5;
            bool ns_finished = (serve_NS==0);
            bool sn_finished = (serve_SN==0);

            WE.write(false);
            EW.write(false);

            NS.write(serve_NS > 0 ? true : false);
            SN.write(serve_SN > 0 ? true : false);

            if (serve_NS > 0) ev_NS_done.notify(serve_NS, SC_SEC);
            if (serve_SN > 0) ev_SN_done.notify(serve_SN, SC_SEC);

            ev_phase_end.notify(5,SC_SEC);

          while (true)
          {
            wait(ev_NS_done | ev_SN_done | ev_phase_end);
        
          if (ev_NS_done.triggered()) {
            cnt_NS -= serve_NS;
            if (cnt_NS < 0) cnt_NS = 0;
            NS.write(false);
            ns_finished = true;
          }

        
          if (ev_SN_done.triggered()) {
            cnt_SN -= serve_SN;
            if (cnt_SN < 0) cnt_SN = 0;
            SN.write(false);
            sn_finished = true;
          }
          if (ns_finished && sn_finished) {
          NS.write(false);
          SN.write(false);
          last_was_ns = true;
          break;
          }

          if (ev_phase_end.triggered()) {
            NS.write(false);
            SN.write(false);
            last_was_ns = true;
            break;
          }
          
        }
      }

        // if WE/EW axis triggered
        if (we_memory)
        {
            int serve_WE = (cnt_WE < 5) ? cnt_WE : 5;
            int serve_EW = (cnt_EW < 5) ? cnt_EW : 5;
            bool we_finished = (serve_WE==0);
            bool ew_finished = (serve_EW==0);

            NS.write(false);
            SN.write(false);

            WE.write(serve_WE > 0 ? true : false);
            EW.write(serve_EW > 0 ? true : false);

            if (serve_WE > 0) ev_WE_done.notify(serve_WE, SC_SEC);
            if (serve_EW > 0) ev_EW_done.notify(serve_EW, SC_SEC);

            ev_phase_end.notify(5,SC_SEC);
          while (true)
          {
            wait(ev_WE_done | ev_EW_done | ev_phase_end);
        
          if (ev_WE_done.triggered() && !we_finished) {
            cnt_WE -= serve_WE;
            if (cnt_WE < 0) cnt_WE = 0;
            WE.write(false);
            we_finished = true;
          }

        
          if (ev_EW_done.triggered() && !ew_finished) {
            cnt_EW -= serve_EW;
            if (cnt_EW < 0) cnt_EW = 0;
            EW.write(false);
            ew_finished = true;
          }
          if (we_finished && ew_finished) {
            WE.write(false);
            EW.write(false);
            last_was_ns = false;
            break;
          }

          if (ev_phase_end.triggered()) {
            WE.write(false);
            EW.write(false);
            last_was_ns = false;
            break;
          }
          
        }
        }
    }
}
