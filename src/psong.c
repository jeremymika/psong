#include <stdio.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 640
#define WinHeight 480
u32 WindowFlags = SDL_WINDOW_OPENGL;

SDL_Window *Window;
SDL_GLContext Context;
b32 Running = 1;
b32 FullScreen = 0;



int main(int argc, char **argv)
{
    printf("Psong (c) 2020 by Jeremy Mika\n");

    Window = SDL_CreateWindow("Psong!", 0, 0, WinWidth, WinHeight, WindowFlags);
    Context = SDL_GL_CreateContext(Window);

    while(Running)
    {
    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_KEYDOWN)
        {
            switch (Event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                Running = 0;
                break;
            case 'f':
                FullScreen = !FullScreen;
                if (FullScreen)
                {
                    SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else
                {
                    SDL_SetWindowFullscreen(Window, WindowFlags);
                }
                break;
            default:
                break;
            }
        }
        else if (Event.type == SDL_QUIT)
        {
            Running = 0;
        }
    }

    glViewport(0, 0, WinWidth, WinHeight);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WinWidth, WinHeight, 0, -10, 10);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    

    SDL_GL_SwapWindow(Window);

    }

    SDL_GL_DeleteContext(Context);

    SDL_DestroyWindow(Window);


    return 0;
}
