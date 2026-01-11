#include <util/TimeHandler.h>

TimeHandler::TimeHandler()
{
    time_when_created = get_absolute_time();
    time_then = time_when_created;
    time_now = time_when_created;
}

void TimeHandler::Update()
{
    time_now = get_absolute_time();   
    delta_us = time_now - time_then;
    dt = static_cast<float>(delta_us) * 1e-6f;
    time_then = time_now;
}