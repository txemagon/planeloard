#include "input.h"
#include "window.h"

#include <SDL2/SDL.h>
#include <strings.h>

Input::Input(Window *window) :
    m_mouse_x(0),
    m_mouse_y(0),
    m_window(window)
{
    bzero (m_inputs, NUM_KEYS * sizeof(bool));
    bzero (m_downkeys, NUM_KEYS * sizeof(bool));
    bzero (m_upkeys, NUM_KEYS * sizeof(bool));

    bzero (m_mouseinput, NUM_MOUSEBUTTONS * sizeof(bool));
    bzero (m_downmouse, NUM_MOUSEBUTTONS * sizeof(bool));
    bzero (m_upmouse, NUM_MOUSEBUTTONS * sizeof(bool));
}

void Input::set_cursor(bool visible) const
{
   SDL_ShowCursor (visible ? 1 : 0);

}

void Input::set_mouse_position(const Vector2f &pos) const
{
    SDL_WarpMouseInWindow(m_window -> get_sdl_window(), (int) pos.get_x(), (int) pos.get_y());
}
