#ifndef __PROFILING_H__
#define __PROFILING_H__

#include <string>

#define PROFILING_DISABLE_MESH_DRAWING 0
#define PROFILING_DISABLE_SHADING      0
#define PROFILING_SET_1x1_VIEWPORT     0
#define PROFILING_SET 2x2_TEXTURE      0

class ProfileTimer
{
    public:
        ProfileTimer() :
            m_num_invocations(0),
            m_total_time(0.0),
            m_start_time(0) {}

        void start_invocation();
        void stop_invocation();

        double display_and_reset(const std::string &message, double divisor = 0, int displayed_message_length = 40);

    protected:
    private:
        int m_num_invocations;
        double m_total_time;
        double m_start_time;
};
#endif
