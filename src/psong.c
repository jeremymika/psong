#include <stdio.h>



#include "SDL2/SDL.h"
//#include "SDL2/SDL_opengl.h"
//#include "GL/glcorearb.h"
#include "GL/gl3w.h"

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 640
#define WinHeight 480
u32 WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

SDL_Window *Window;
SDL_GLContext Context;
b32 Running = 1;
b32 FullScreen = 0;


typedef float t_mat4x4[16];

static inline void mat4x4_ortho( t_mat4x4 out, float left, float right, float bottom, float top, float znear, float zfar )
{
    #define T(a, b) (a * 4 + b)

    out[T(0,0)] = 2.0f / (right - left);
    out[T(0,1)] = 0.0f;
    out[T(0,2)] = 0.0f;
    out[T(0,3)] = 0.0f;

    out[T(1,1)] = 2.0f / (top - bottom);
    out[T(1,0)] = 0.0f;
    out[T(1,2)] = 0.0f;
    out[T(1,3)] = 0.0f;

    out[T(2,2)] = -2.0f / (zfar - znear);
    out[T(2,0)] = 0.0f;
    out[T(2,1)] = 0.0f;
    out[T(2,3)] = 0.0f;

    out[T(3,0)] = -(right + left) / (right - left);
    out[T(3,1)] = -(top + bottom) / (top - bottom);
    out[T(3,2)] = -(zfar + znear) / (zfar - znear);
    out[T(3,3)] = 1.0f;

    #undef T
}

static const char * vertex_shader =
    "#version 150\n"
    "in vec2 i_position;\n"
    "in vec4 i_color;\n"
    "out vec4 v_color;\n"
    "uniform mat4 u_projection_matrix;\n"
    "void main() {\n"
    "    v_color = i_color;\n"
    "    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );\n"
    "}\n";

static const char * fragment_shader =
    "#version 150\n"
    "in vec4 v_color;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = v_color;\n"
    "}\n";

typedef enum t_attrib_id
{
    attrib_position,
    attrib_color
} t_attrib_id;


/// GLSL version string.
static const char* g_glsl_version;

int main(int argc, char **argv)
{
    printf("Psong (c) 2020 by Jeremy Mika\n");
    SDL_Init( SDL_INIT_EVERYTHING );

    g_glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
 
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );



    Window = SDL_CreateWindow("Psong!", 100, 100, WinWidth, WinHeight, WindowFlags);
    Context = SDL_GL_CreateContext(Window);


    if (gl3wInit()) {
            fprintf(stderr, "failed to initialize OpenGL\n");
            return -1;
    }
    if (!gl3wIsSupported(3, 2)) {
            fprintf(stderr, "OpenGL 3.2 not supported\n");
            return -1;
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
            glGetString(GL_SHADING_LANGUAGE_VERSION));

    GLuint vs, fs, program;

    vs = glCreateShader( GL_VERTEX_SHADER );
    fs = glCreateShader( GL_FRAGMENT_SHADER );

    int length = strlen( vertex_shader );
    glShaderSource( vs, 1, ( const GLchar ** )&vertex_shader, &length );
    glCompileShader( vs );

    GLint status;
    glGetShaderiv( vs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        fprintf( stderr, "vertex shader compilation failed\n" );
        GLint maxLength = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLchar *errorLog = (GLchar *)malloc(maxLength);
        glGetShaderInfoLog(vs, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.

        printf(errorLog);

        free(errorLog);

        // Exit with failure.
        glDeleteShader(vs); // Don't leak the shader.        return 1;
    }

    length = strlen( fragment_shader );
    glShaderSource( fs, 1, ( const GLchar ** )&fragment_shader, &length );
    glCompileShader( fs );

    glGetShaderiv( fs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        fprintf( stderr, "fragment shader compilation failed\n" );
        return 1;
    }

    program = glCreateProgram();
    glAttachShader( program, vs );
    glAttachShader( program, fs );

    glBindAttribLocation( program, attrib_position, "i_position" );
    glBindAttribLocation( program, attrib_color, "i_color" );
    glLinkProgram( program );

    glUseProgram( program );

    GLuint vao, vbo;

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glEnableVertexAttribArray( attrib_position );
    glEnableVertexAttribArray( attrib_color );

    glVertexAttribPointer( attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
    glVertexAttribPointer( attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * )(4 * sizeof(float)) );

    const GLfloat g_vertex_buffer_data[] = {
    /*  R, G, B, A, X, Y  */
        1, 0, 0, 1, 0, 0,
        0, 1, 0, 1, WinWidth, 0,
        0, 0, 1, 1, WinWidth, WinHeight,

        1, 0, 0, 1, 0, 0,
        0, 0, 1, 1, WinWidth, WinHeight,
        1, 1, 1, 1, 0, WinHeight
    };

    glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );

    t_mat4x4 projection_matrix;
    mat4x4_ortho( projection_matrix, 0.0f, (float)WinWidth, (float)WinHeight, 0.0f, 0.0f, 100.0f );
    glUniformMatrix4fv( glGetUniformLocation( program, "u_projection_matrix" ), 1, GL_FALSE, projection_matrix );

    while(Running)
    {
    SDL_Event Event;

    glClear( GL_COLOR_BUFFER_BIT );

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


        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES, 0, 6 );

        SDL_GL_SwapWindow( Window );
  /*      
    glViewport(0, 0, WinWidth, WinHeight);
    glDisable( GL_DEPTH_TEST );
    glClearColor(0.1f, 0.1f, 0.3f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WinWidth, WinHeight, 0, -10, 10);

    //Back to the modelview so we can draw stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    */


    }

    SDL_GL_DeleteContext(Context);

    SDL_DestroyWindow(Window);


    return 0;
}
