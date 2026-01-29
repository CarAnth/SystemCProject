#include <iostream>
#include "traffic.h"

using std::cout;
using std::endl;

TrafficLight::TrafficLight(sc_module_name name, sc_time green_hold_time)
: sc_module(name),
  green_hold(green_hold_time),
  is_green(false)
{
    light.initialize(RED);

    SC_METHOD(on_grant_method);
    dont_initialize();
    sensitive << grant_green;

    SC_METHOD(on_car_change_method);
    dont_initialize();
    sensitive << car_present;

    SC_THREAD(green_timer_thread);
}

void TrafficLight::on_grant_method()
{
    if (grant_green.read() == true)
    {
        is_green = true;
        light.write(GREEN);

        cout << sc_time_stamp() << " [" << name() << "] -> GREEN"
             << " (car_present=" << (car_present.read() ? "1" : "0") << ")"
             << endl;

        // start/restart timer after update phase (next delta)
        start_green_timer_ev.notify(SC_ZERO_TIME);
    }
}

void TrafficLight::on_car_change_method()
{
    cout << sc_time_stamp() << " [" << name() << "] car_present="
         << (car_present.read() ? "1" : "0")
         << " | light=" << LightToStr(light.read())
         << endl;
}

void TrafficLight::green_timer_thread()
{
    for (;;)
    {
        wait(start_green_timer_ev);
        wait(green_hold);

        is_green = false;
        light.write(RED);

        cout << sc_time_stamp() << " [" << name() << "] -> RED (green_hold done)" << endl;
    }
}
