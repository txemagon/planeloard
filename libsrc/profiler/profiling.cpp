#include "profiling.h"
#include "timing.h"
#include <cassert>
#include <iostream>

void ProfileTimer::start_invocation() { m_start_time = get_time(); }

void ProfileTimer::stop_invocation()
{
   if (m_start_time)
   {
       std::cout << "Error: stop invocation called without matching start_invocation." <<std::endl;
       assert (m_start_time != 0);
   }

   m_num_invocations++;
   m_total_time += (Time::GetTime() - m_start_time);
   m_start_time = 0;
}

double ProfileTimer::get_time_and_reset(double divisor)
{
    divisor = (divisor == 0) ? m_num_invocations : divisor;
    double result = (m_total_time == 0 && divisor == 0.0) ? 0.0 : (1000 * m_total_time) / (double) divisor;
    m_total_time = 0.0;
    m_num_invocations = 0;

    return result;
}

double ProfileTimer::display_and_reset(const std::string &message, double divisor, int displayed_message_length)
{
    std::string white_space = "";

    for (int i=message.length(); i< displayed_message_length; i++)
        white_space += " ";

    double time = get_time_and_reset(divisor);
    std::cout << message << white_space << time << " ms" << std::endl;

    return time;
}


