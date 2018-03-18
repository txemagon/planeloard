#ifndef __CORE_ENGINE_H__
#define __CORE_ENGINE_H__

class Engine
{
    public:
        Engine(double frame_rate, Window *window, RenderingEngine rendering_engine, Simulation *simulation);

        void start();
        void stop();

        inline RenderingEngine *get_rendering_engine() { return m_rendering_engine; }
    protected:
    private:
        bool             m_is_running;
        double           m_frame_time;
        Window          *m_window;
        RenderingEngine *m_rendering_engine;
        Simulation      *m_simulation;
}

#endif
