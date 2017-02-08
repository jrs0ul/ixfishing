#ifdef WIN32
    #ifdef  _MSC_VER
        #define _CRT_SECURE_NO_DEPRECATE 1
        #pragma comment(lib,"SDL.lib")
        #pragma comment(lib,"SDLmain.lib")
        #pragma comment(lib,"OpenGl32.lib")
        #pragma comment(lib,"GLU32.lib")
        #pragma comment(lib,"openal32.lib")

        #ifdef _DEBUG
            #pragma comment(lib,"ogg_d.lib")
            #pragma comment(lib,"vorbis_d.lib")
            #pragma comment(lib,"vorbisfile_d.lib")
        #else
            #pragma comment(lib,"ogg.lib")
            #pragma comment(lib,"vorbis.lib")
            #pragma comment(lib,"vorbisfile.lib")
        #endif
    #endif
#endif



#include <ctime>
#include "CppSingleton.h"
#include "SDLVideo.h"
#include "Utils.h"
#ifdef __APPLE__
#include <limits.h>
#include <unistd.h>
#include <CoreFoundation/CoreFoundation.h>
#endif


//const int WIDTH = 800;
//const int HEIGHT = 600;
//bool windowed = false;

SDLVideo SDL;
SDL_Joystick *Joy = 0;
int JoyX = 0;
int JoyY = 0;
int MouseX, MouseY; //relative mouse coords
int _MouseX, _MouseY;
unsigned long tick;

Singleton Game;


void ConfGL(){
    Game.init();
}
//-----------------
void RenderScreen(){
    Game.render();
    glFlush();

    SDL_GL_SwapBuffers( );
}
//-----------------
void Logic(){
    Game.logic();
}
//-----------------
static void  process_events(){
    
    SDL_Event event;
    float scaleX = 480.0f/Game.ScreenWidth;
    float scaleY = 320.0f/Game.ScreenHeight;

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {

        case SDL_KEYDOWN:{

            Game.globalKEY = (char)event.key.keysym.unicode;
            switch( event.key.keysym.sym ) {
                default:{} break;
                case SDLK_q: {Game.saveMyData(); Game.Works = false;}
            }
        } break;
        case SDL_MOUSEBUTTONUP:{
            Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
            Game.touches.up.add(pos);
            Game.touches.allfingersup = true;
        } break;
        case SDL_MOUSEBUTTONDOWN:{
            Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
            Game.touches.down.add(pos);
            Game.touches.allfingersup = false;

        } break;
        case SDL_MOUSEMOTION:{
            if(SDL_GetMouseState(0, 0)&SDL_BUTTON_LMASK){
                Vector3D pos(event.button.x * scaleX, event.button.y * scaleY, 0);
                Game.touches.move.add(pos);
                Game.touches.allfingersup = false;
            }
        }break;


        case SDL_QUIT:{
            Game.saveMyData();
            Game.Works = false;
        }break;
    
        }

    }
}
//--------------------
void checkKeys(){
    float scaleX = 480.0f/Game.ScreenWidth;
    float scaleY = 320.0f/Game.ScreenHeight;
    Uint8 * keys;
    int JoyNum = 0;
    
    keys = SDL_GetKeyState ( NULL );
    JoyNum = SDL_NumJoysticks();

    if (JoyNum > 0) {

        //printf("%s\n", SDL_JoystickName(0));

        SDL_JoystickUpdate ();
        JoyX = SDL_JoystickGetAxis(Joy, 0);
        JoyY = SDL_JoystickGetAxis(Joy, 1);
    }

    int bm;
    bm = SDL_GetRelativeMouseState ( &MouseX,&MouseY );
    SDL_GetMouseState(&_MouseX, &_MouseY);

    Game.gamepad.v[0] = JoyX/ 1000.0f;
    Game.gamepad.v[1] = JoyY/ 1000.0f;
    
    Game.MouseX = _MouseX*scaleX;
    Game.MouseY = _MouseY*scaleY;

    //Game.gamepad.v[0] = MouseX * 10.0f;
    //Game.gamepad.v[1] = MouseY * 10.0f;


    memcpy(Game.OldKeys, Game.Keys, 20);
    memset(Game.Keys, 0, 20);

    if ( keys[SDLK_w] )    Game.Keys[0] = 1;
    if ( keys[SDLK_s] )    Game.Keys[1] = 1;
    if ( keys[SDLK_a] )    Game.Keys[2] = 1;
    if ( keys[SDLK_d] )    Game.Keys[3] = 1;
    if ( keys[SDLK_UP] )   Game.Keys[0] = 1;
    if ( keys[SDLK_DOWN])  Game.Keys[1] = 1;
    if ( keys[SDLK_LEFT])  Game.Keys[2] = 1;
    if ( keys[SDLK_RIGHT]) Game.Keys[3] = 1;
    if ( keys[SDLK_SPACE]) Game.Keys[4] = 1;
    if ( keys[SDLK_RETURN]) Game.Keys[5] = 1;
    if ( keys[SDLK_ESCAPE]) Game.Keys[6] = 1;

    if (JoyNum){
        if (SDL_JoystickGetButton (Joy, 0))
            Game.Keys[4] = 1;
        if (SDL_JoystickGetButton (Joy, 1))
            Game.Keys[5] = 1;
    }
}
//--------------------
int main( int   argc, char *argv[] ){
    
    srand(time(0));

    char buf[255];
    GetHomePath(buf);
    sprintf(Game.DocumentPath, "%s.IXFishing", buf);
    MakeDir(Game.DocumentPath);
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX)){
        puts("Um...Error?");
    }
    CFRelease(resourcesURL);
    chdir(path);
#endif
    Game.loadConfig();

    printf("%d %d\n", Game.ScreenWidth, Game.ScreenHeight);
    SDL.setMetrics(Game.ScreenWidth, Game.ScreenHeight);
    if (!SDL.InitWindow("IXFishing 1.30", "icon.bmp", Game.windowed)){
        Game.Works = false;
    }

    if (!Game.windowed)
        SDL_ShowCursor(SDL_DISABLE);

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    if(SDL_NumJoysticks() > 0){
        Joy = SDL_JoystickOpen(0);
    }
    SDL_EnableUNICODE(SDL_ENABLE);
    ConfGL();
    //LoadExtensions();
    

     while (Game.Works){
        if ((SDL_GetTicks() > tick)){

            checkKeys();
            Logic();
            tick = SDL_GetTicks() + 13;
        }
        SDL_Delay(1);

        RenderScreen();
        process_events();

        if (Game.launchmystore){
            Game.launchmystore = false;
#ifdef __APPLE__
            system("open -a Safari http://github.com/jrs0ul/");
#else
    #ifdef WIN32
            system("explorer http://github.com/jrs0ul");
    #else
            if (!system("firefox https://github.com/jrs0ul"))
                puts("eat shit and die");
    #endif

#endif
        }


    }

    Game.destroy();

    SDL.Quit();

    return 0;
}

