#ifndef __INPUT_H__
#define __INPUT_H__

#include "../math/math3d.h"

class Window;

class Input
{
    public:
#include "usb_mouse.h"
#include "usb_keyb.h"

        static const int NUM_KEYS = 512;
        static const int NUM_MOUSEBUTTONS = 256;

        Input(Window *window);

        inline bool get_key(int keycode)               const { return m_inputs[keycode]; }
        inline bool get_key_down(int keycode)          const { return m_downkeys[keycode]; }
        inline bool get_key_up(int keycode)            const { return m_upkeys[keycode]; }
        inline bool get_mouse(int keycode)             const { return m_mouseinput[keycode]; }
        inline bool get_mouse_down(int keycode)        const { return m_downmouse[keycode]; }
        inline bool get_mouse_up(int keycode)          const { return m_upmouse[keycode]; }
        inline Vector2f get_mouse_position()           const { return Vector2f((float) m_mouse_x, (float) m_mouse_y); }

        void set_cursor(bool visible)                  const;
        void set_mouse_position(const Vector2f &value)            const;

        inline void set_key(int keycode, bool value)         { m_inputs[keycode] = value; }
        inline void set_key_down(int keycode, bool value)    { m_downkeys[keycode] = value; }
        inline void set_key_up(int keycode, bool value)      { m_upkeys[keycode] = value; }
        inline void set_mouse(int keycode, bool value)       { m_mouseinput[keycode] = value; }
        inline void set_mouse_down(int keycode, bool value)  { m_downmouse[keycode] = value; }
        inline void set_mouse_up(int keycode, bool value)    { m_upmouse[keycode] = value; }
        inline void set_mouse_x(int value)                   { m_mouse_x = value; }
        inline void set_mouse_y(int value)                   { m_mouse_y = value; }

    protected:
    private:
        bool m_inputs[NUM_KEYS];
        bool m_downkeys[NUM_KEYS];
        bool m_upkeys[NUM_KEYS];
        bool m_mouseinput[NUM_MOUSEBUTTONS];
        bool m_downmouse[NUM_MOUSEBUTTONS];
        bool m_upmouse[NUM_MOUSEBUTTONS];
        int  m_mouse_x, m_mouse_y;
        Window *m_window;

};

#endif
