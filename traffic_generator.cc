#include "traffic_generator.h"

int TrafficGenerator::next_delay_sec()
{
    return(rand()%4)+1;//rand()%4=0,1,2,3 with + 1 = 1,2,3,4.
}

TrafficGenerator::TrafficGenerator(sc_module_name name,
                                   sc_event &ns,
                                   sc_event &sn,
                                   sc_event &we,
                                   sc_event &ew)
    :sc_module(name)
{
    ev_NS = &ns;//defining pointers
    ev_SN = &sn;
    ev_EW = &ew;
    ev_WE = &we;

    SC_THREAD(generate_NS);//we are using wait()function
    SC_THREAD(generate_SN);//4 independent function
    SC_THREAD(generate_EW);
    SC_THREAD(generate_WE);
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
