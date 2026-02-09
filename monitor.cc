#include "monitor.h"
#include <iostream>
#include <cassert>


using std::cout;//we don't need to write std:: prefix
using std::endl;

const char* color(int v){
    return (v == 1) ? "GREEN":"RED";//more visual output
}


Monitor::Monitor(sc_module_name name)
: sc_module(name)
{
    SC_METHOD(check_and_print);
    dont_initialize();//first trigger  when signal changes,not t=0
    sensitive << NS << SN << WE << EW;// sensitive for axises
}

void Monitor::check_and_print()//observing outputs
{

    cout << sc_time_stamp()//writing log
         << " | NS=" << color(NS.read())
         << " SN=" << color(SN.read())
	 << " WE=" << color(WE.read())
	 << " EW=" << color(EW.read())
	 << endl;

    bool ns_axis_green = (NS.read() == 1) || (SN.read() == 1);//safety assert for nx_axis
    bool we_axis_green = (WE.read() == 1) || (EW.read() == 1);//safety assert for we_axis
    assert(!(ns_axis_green && we_axis_green));//detecting safety errors
}
