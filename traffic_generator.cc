#include "traffic_generator.h"
using std::cout;

int TrafficGenerator::next_delay_sec()
{
    if (mode == 0)
    {
        return(rand()%4)+1;//rand()%4=0,1,2,3 with + 1 = 1,2,3,4.
    }else{
        return 1;//targeted
    }
    
    
}

TrafficGenerator::TrafficGenerator(sc_module_name name,
                                   sc_event &ns,
                                   sc_event &sn,
                                   sc_event &we,
                                   sc_event &ew,
                                   int mode)
    :sc_module(name)
{
    this->mode = mode;

    ev_NS = &ns;//defining pointers
    ev_SN = &sn;
    ev_EW = &ew;
    ev_WE = &we;

    if(mode==0){
        SC_THREAD(generate_NS);//we are using wait()function
        SC_THREAD(generate_SN);//4 independent function
        SC_THREAD(generate_EW);
        SC_THREAD(generate_WE);
    }else if(mode==1){
        SC_THREAD(targeted_traffic);
    }
}

void TrafficGenerator::generate_NS(){
    while(true){
        wait(next_delay_sec(),SC_SEC);
        ev_NS->notify();
    }

}

void TrafficGenerator::generate_SN(){
    while(true){
        wait(next_delay_sec(),SC_SEC);
        ev_SN->notify();
    }    
}

void TrafficGenerator::generate_WE(){
    while(true){
        wait(next_delay_sec(),SC_SEC);
        ev_WE->notify();
    }
}

void TrafficGenerator::generate_EW(){
    while(true){
        wait(next_delay_sec(),SC_SEC);
        ev_EW->notify();
    }    
}

void TrafficGenerator::targeted_traffic()
{
    while (true)
    {
        //single direction
        ev_SN->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);

        ev_NS->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);
        
        ev_WE->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);

        ev_EW->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);
        
        //parallel direction
        ev_SN->notify(SC_ZERO_TIME);
        ev_NS->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);
        
        ev_WE->notify(SC_ZERO_TIME);
        ev_EW->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);

        //perpendicular
        ev_NS->notify(SC_ZERO_TIME);
        ev_WE->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);
        
        ev_SN->notify(SC_ZERO_TIME);
        ev_EW->notify(SC_ZERO_TIME);
        wait(5,SC_SEC);
    }
    
}
