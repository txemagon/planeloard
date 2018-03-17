#include <stdio.h>

#include "engine.h"
#include "timing.h"
#include "../interface/window.h"
#include "../interface/input.h"
#include "util.h"
#include "simulation.h"


#define DEBUG_TIME 1.0

Engine::Engine(double frame_rate, Window *window, RenderingEngine *rendering_engine, Simulation *simulation) :
    m_is_running (false),
    m_frame_time (1.0 / frame_rate),
    m_window (window),
    m_rendering_engine (rendering_engine),
    m_simulation (simulation)
{
    m_simulation -> set_engine (this);
    m_simulation -> init (*m_window)
}

void Engine::start()
{
    if (m_is_running)
        return;

    m_is_running = true;

    double last_time = Time::GetTime();
    double unprocessed_time = 0;

    double since_frame_counter = 0;
    int frames = 0;                      // Frames pending to render

    ProfileTimer sleep_timer;
    ProfileTimer swap_buffer_timer;
    ProfileTimer window_update_timer;

    while (m_is_running)
    {
        bool render = false;
        double start_time = Time::GetTime();
        double passed_time = start_time - last_time;

        unprocessed_time += passed_time;
        since_frame_counter += passed_time;


        if (since_frame_counter >= DEBUG_TIME)
        {
            double total_time = (1000.0 * since_frame_counter) / (double) frames );
            double total_measured_time = 0.0;

            total_measured_time += m_simulation -> display_input_time ((double) frames);
            total_measured_time += m_simulation -> display_update_time ((double) frames);
            total_measured_time += m_rendering_engine -> display_render_time ((double) frames);
            total_measured_time += sleep_timer. display_and_reset ("Sleep Time: ", (double) frames);
            total_measured_time += window_update_timer.display_and_reset ("Window Update Time: ", (double) frames);
            total_measured_time += swap_buffer_timer.display_and_reset ("Buffer Swap Time", (double) frames);
            total_measured_time += m_rendering_engine -> display_window_sync_time ( (double) frames );

            printf("Other time:                        %f ms\n", total_time - total_measured_time);
            printf("Total time:                        %f ms\n", total_time);

            frames = 0;
            since_frame_counter = 0;
        }

        while (unprocessed_time > m_frame_time)
        {
            window_update_timer.start_invocation ();
            m_window.update ();

            if (m_window -> is_closed_request ())
                stop ();
            window_update_timer.stop_invocation ();

            m_simulation -> process_input (m_window -> get_input (), float m_frame_time);
            m_simulation -> update (m_frame_time);

            render = true;
            unprocessed_time -= m_frame_time;
        }

        if (render)
        {
            m_simulation -> render (m_rendering_engine);
            swap_buffer_timer.start_invocation ();
            m_window->swap_buffers ();
            swap_buffer_timer.stop_invocation ();
            frames ++;
        }
        else
        {
            sleep_timer.start_invocation ();
            Util::sleep(1);
            sleep_timer.stop_invocation ();
        }
    }
}

void Engine::stop()
{
    m_is_running = false;
}
