#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL2/SDL.h>
#include <string>
#include "input.h"

class Window
{
    public:
        Window(int width, int height, const std::string &title);
        virtual ~Window();

        void update();
        void swap_buffers();
        void bind_as_render_target() const;

        inline bool is_close_requested()       const { return m_is_closed_requested; }
        inline int  get_width()                const { return m_width; }
        inline int  get_height()               const { return m_height; }
        inline float get_aspet()               const { return (float) m_width / m_height; }
        inline const std::string &get_title()  const { return m_title; }
        inline Vector2f get_center()           const { return Vector2f( m_width / 2., m_height / 2. ); }
        inline SDL_Window *get_SDL_window()          { return m_window; }
        inline const Input &get_input()        const { return m_input; }

        void set_full_screen(bool value);

    protected:
    private:
        int            m_width;
        int            m_height;
        std::string    m_title;
        SDL_Window    *m_window;
        SDL_GLContext  m_gl_context;
        Input          m_input;
        bool           m_is_closed_requested;

        Window(const Window &other) : m_input(this) {}
        void operator=(const Window &other) {}
};

#endif
