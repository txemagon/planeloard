#include "window.h"
#include "..profiler/profiler.h
#include "..profiler/profiling.h""

#include <SDL2/SDL.h>
#include <GL/glew.h>

Window::Window(int width, int height, const std::string &title) :
    m_width(width),
    m_height(height),
    m_title(title),
    m_input(this),
    m_is_closed_requested(false)
{
    SDL_Init (SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);


    SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 2);

    m_window = SDL_CreateWindow (title.c_str(),
                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 width, height, SDL_WINDOW_OPENGL);

    m_gl_context = SDL_GL_CreateContext (m_window);

    SDL_GL_SetSwapInterval(1);

    // for xcode
    glewExperimental = TRUE;

    GLenum res = glewInit();
    if (res != GLEW_OK)
        fprintf (stderr, "Error: '%s'\n", glewGetErrorString(res));
}

Window::~Window()
{
    SDL_GL_DeleteContext (m_gl_context);
    SDL_DestroyWindow (m_window);
    SDL_Quit ();
}

void Window::update()
{
    for (int i=0; i<Input::NUM_MOUSEBUTTONS; i++)
    {
        m_input.set_mouse_down(i, false);
        m_input.set_mouse_up(i, false);
    }

    for (int i=0; i<Input::NUM_KEYS; i++)
    {
        m_input.set_key_down(i, false);
        m_input.set_key_up(i, false);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        int value = 0;
        switch(e.type)
        {
            case SDL_QUIT:
                m_is_closed_requested = true;
                break;
            case SDL_MOUSEMOTION:
                m_input.set_mouse_x(e.motion.x);
                m_input.set_mouse_y(e.motion.y);
                break;
            case SDL_KEYDOWN:
                value = e.key.keysym.scancode;

                m_input.set_key(value, true);
                m_input.set_key_down(value, true);
                break;
            case SDL_KEYUP:
                value = e.key.keysym.scancode;

                m_input.set_key(value, false);
                m_input.set_key_up(value, true);
                break;
            case SDL_MOUSEBUTTONDOWN:
                value = e.button.button;

                m_input.set_mouse(value, true);
                m_input.set_mouse_down(value, true);
                break;
            case SDL_MOUSEBUTTONUP:
                value = e.button.button;

                m_input.set_mouse(value, false);
                m_input.set_mouse_up(value, true);
                break;
        }
    }
}

void Window::swap_buffers()
{
    SDL_GL_SwapWindow (m_window);
}

void Window::bind_as_render_target() const
{
    glBindTexture (GL_TEXTURE_2D, 0);
    gl_BindFrameBuffer (GL_FRAMEBUFFER, 0);

#if PROFILING_SET_1x1_VIEWPORT == 0
    glViewport(0, 0, get_width(), get_height());
#else
    glViewport(0, 0, 1, 1);
#endif
}

void Window::set_full_screen(bool value)
{
    int mode = value ? SDL_WINDOW_FULLSCREEN : 0;
    SDL_SetWindowFullscreen (m_window, mode);
}
