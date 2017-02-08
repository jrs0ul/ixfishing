#ifndef _CPP_SINGLETON
#define _CPP_SINGLETON

#include <ctime>
#include "audio/SoundSystem.h"
#include "audio/OggStream.h"
#include "TextureLoader.h"
#include "Vectors.h"
#include "TouchData.h"
#include "fCommon.h"
#include "Fish.h"
#include "gui/Button.h"
#include "Baits.h"
#include "MyData.h"
#include "gui/Text.h"
#include "gui/YesNoDialog.h"
#include "gui/Scrollbar.h"
#include "SysConfig.h"
#include "Collisions.h"
#include "Utils.h"
#include "Threads.h"
#include "externals.h"
#include "FindPath.h"
#include "Scene.h"
#include "Lake.h"
//------------------------------------------------
struct TDude{
    int x;
    int y;
    int tic;
    float scale;
    unsigned frame;
    
    TDude(){
        tic = 0;
        frame = 1;
        scale = 0.5f;
    }
};
//---------------------------------------
struct THand{
    int tim;
    bool rollin;
    bool rollout;
    bool display;
    int x;
    
    THand(){
        reset();
    }
    void reset(){
        x = -128; rollout = false;
        tim = 0; rollin = false; display = false;
    }
};
//------------------------
struct Rod{
    int depth; 
    
    unsigned floatFrame;
    int floatFrameTics;
    
};

//-------------------------------------
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
#define MAXPOWER 90
#define MAXFISHY 180
#define MINFISHX 30
#define MAXFISHX 450

const int CLOUDCOUNT = 8;


const int UZMETSTARTX = (MAXFISHX - MINFISHX)/2 + MINFISHX;
const int LINESTARTXL = UZMETSTARTX - 32; //fisherman position x;
const int LINESTARTXR = LINESTARTXL + 64;


#ifndef __IPHONEOS__
//   #define _DEBUG_ 1
#endif

const int krepsis_max = 10000;

enum GameStates{INTRO, TOWN, LAKE, STORE, SELLPOINT, CREDITS, HELP, OPTIONS, LAKESELECT, BOAT, BUS, CAT};

const int LR_WIDTH = 90;
const int LR_HEIGHT = 30; 

const COLOR PANELCOLOR(0.66f, 0.66f, 0.66f, 0.8f);

const int DepthDialogWidth = 220;
const int DepthDialogHeigt = 150;
const int BAITHEIGHT = 50;
const int LineTensionMax = 50;

const unsigned maxHelpPage = 3;

const int TicketItem = 40;



//============================================
class Singleton{
    
    void LoadFiles();
    void draw_intro();
    //void CreateFishTypes();
    void checkconfig(const char *name);
    void loadlang();
    void drawBus();
    void busLogic();
    void drawCat();
    void catLogic();
    void DrawAdventureMode(unsigned index, bool debug=false);
    void showstatus(int x, int y, unsigned type = 0);
    void mainMenuLogic();
    void DrawClouds();
    void GenerateClouds(int horizon);
    void AdventureLogic();
    void Randomfish(FishPack& pack);
    void MoveClouds(int horizon);
    void LakeLogic();
    void DrawLake();
    COLOR MapDepthColor(int depth);
    void DrawLine(float x1, float y1, float x2, float y2);
    void drawFishXY(int x, int y);
    void xbox(PicsContainer& pics, int x, int y, int x1, int y1, COLOR c);
    void DrawLRBar(int x, int y);
    void DrawPowerBar(int x, int y,
                      COLOR color);
    void DrawWindBar(int x, int y);
    void DrawHandnFish(int x, int y);
    void DrawDepthCfg();
    void DrawBaitCfg();
    void DrawBucket();
    void HandAnimation();
    void baitCfgLogic();
    void DepthConfigLogic();
    void onBuckit();
    void MoveRandomly(Fish* z);
    void FishAI();
    void fish_atc(unsigned index);
    bool fishSmellsBait(unsigned fishindex, unsigned fishlikeswhat);
    void CheckPower(int max);
    void CheckPullout();
    void CheckLRButtons();
    void PullOut();
    void Throw();
   
    void punktas();
    void DrawRestaurant();
    void showfish(int x, int y, int c, int ind);
    
    void ShopLogic();
    void DrawShop();
    
    void DrawCredits();
    void Credits();
    void DrawHelp();
    void Help();
    void optionsLogic();
    void drawOptions();
    void DrawTensionBar(int x, int y);
    void drawFishingSpotSelection();
    void fishingSpotSelectionLogic();
    void DrawCoolMetterFrame(int x, int y, unsigned width, unsigned height);
    void enterLake();
    void LoadLakeImages();
    void drawBoat();
    void boatLogic();
    void makePath(unsigned sceneIndex, int x, int y);

    void enterBoat();
    static void* _enterBoat(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterBoat();
        return 0;
    }


    void enterCat();
    static void* _enterCat(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterCat();
        return 0;
    }



    void enterLakeSelect();
    static void* _enterLakeSelect(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterLakeSelect();
        return 0;
    }

    void enterTown();
    static void* _enterTown(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterTown();
        return 0;
    }

    void enterBus();
    static void* _enterBus(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterBus();
        return 0;
    }



    void enterHome();
    static void* _enterHome(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterHome();
        return 0;
    }




    void enterCity();
    static void* _enterCity(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterCity();
        return 0;
    }

    void enterBaitShop();
    static void* _enterBaitShop(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterBaitShop();
        return 0;
    }


    //--
    void enterSellPoint();
    static void* _enterSellPoint(void *arg){
        reinterpret_cast<Singleton*>(arg)->enterSellPoint();
        return 0;
    }
    //--

    void exitLake();

    static THREADFUNC ThreadFunc(void* arg){
        reinterpret_cast<Singleton*>(arg)->LoadLakeImages();
        return 0;
    }
public:
   

    int ScreenWidth;
    int ScreenHeight;
    bool windowed;

    char DocumentPath[255];

    unsigned char Keys[20];
    unsigned char OldKeys[20];
    Vector3D gamepad;
    
   
    Lake lakes[2];
    unsigned lakeIndex;

    bool islandEnabled;
    
    SoundSystem ss;
    OggStream music;
    
    PicsContainer pics;
    PicsContainer LakeGfx;
    
    
    GameStates GameState;
    
    int LineStartX;
    int LineStartY;
    
    int Ftim;
    
    bool throwAnimation;
    int throwAnimationTic;
    unsigned ZvejasFrame;
    
    bool MasalasCfg_Active;
    bool GylisCfg_Active;
    bool Buckit_Active;
    bool ExitDialog_Active;
    
    
    float Alpha1;
    float Alpha2;
    bool WaterSwitch;
    
    int Clouds[4*CLOUDCOUNT];
    TFont Font;
    
    FishData fishData;
    DArray<Fish> fishes;
    
    int _bait;
    
    char lang[26][50];
    
    char lang_def[50];
    
    BaitData baitData;
    
    
    bool fingerOnLR;
    
    bool tooMuchFishMessageActive;
    
    
    int MouseXOld, MouseYOld, MouseX, MouseY, MouseButton, MouseButtonOld,
    fishx,fishy;
    
    Rod MyRod;
    
    Vector3D oldMoveTouch;
    
    MyData me;
    int LineTension;
    
    
    TDude Dude;
    int TikslasX, TikslasY,
    pludex,pludey,
    krepsis_def,power,lr,wind,wi;
    
    bool done, pushed, Exit1, uzkibo, nustatyta;
    
    char Key, OldKey;
    
    THand hand;
    
    unsigned masalas, masalasold, zuvID;
    int preke;

    bool uzmesta;
    bool uzsimota;
    bool pakirsta;
    bool finishedPullOut;
    
    Vector3D thrownPoint;
    Vector3D thrownAnimPoint;
    Vector3D throwDir;
    
    Button shop_exit;
    Button shop_buy;
    char shopKeeperSays[100];
    
    unsigned globalKEY;
    
    Button Restaurant_exit;
    Button Restaurant_sell;
    Button lake_exit;
    Button lake_depth;
    Button lake_buckit;
    Button lake_throw;
    Button lake_bait;
    
    Button boatNo_button;
    Button boatYes_button;
    Button boatExit_button;

    Button catExit_button;
    Button catGive_button;

    Button busExit_button;
    Button busGo_button;

    Button lake_LRBar;
    
    Button lake_DepthBar;
    Button BucketWindow;
    Button jrs0ul_button;
    Button Depth_OK;
    Button BucketWindow_OK;
    Button Bait_CfgWindow;
    Button Bait_Area;
    Button Bait_OK;
    Button Play_Button;
    Button Credits_Button;
    Button Help_Button;
    Button Options_Button;
    Button QuitButton;
    Button resetSaveGame_button;
    Button linetension_button;
    Button optionsOK_button;
    Button Shop_GoodsArea;
    Button Rest_FishArea;
    Button BusTicketArea;
    YesNoDialog yesno;
    Button more_button;
    Button prevHelpPage_button;
    Button nextHelpPage_button;
    Button helpBack_button;
    Button NoMusic_Button;
    
    
    
    
    unsigned activeHelpPage;
    char boatMan[3][100];
    char busDriver[3][100];
    unsigned busDriverSpeachIndex;
    
    ScrollBar musicVolumeBar;
    
    bool useAccel;
    Vector3D _acceleration;
    Vector3D firstTouch;
    Vector3D pos;
    
    SystemConfig sys;
    
    int clickCount;
    
    bool launchjrs0ul;
    bool launchmystore;

    TouchData touches;
    
    int fishingSpotsAnim;
    bool fishingSpotAnimationDir;
    
    char LakeGfxList[10][255];
    Image LakePicsData[10];
    bool loadingLakePics;
    bool lakePicsLoaded;
    int loadingAnim;

    unsigned ticketIndex;

    bool Works;

    //---
    DArray<Scene> Scenes;
    unsigned SceneIndex;

    Path walkPath;
    unsigned pathStep;

    bool gotCatFishes;

    
    
    Singleton():shop_exit(420, 275, 60, 45,COLOR(0.7f, 0.7f, 1.0f)),
                shop_buy(350, 275, 60, 45, COLOR(0.7f, 0.7f, 1.0f)),
                Restaurant_exit(220, 275, 60, 45, COLOR(0.7f, 0.7f, 1.0f)),
                Restaurant_sell(150, 275, 60, 45, COLOR(0.7f, 0.7f, 1.0f)),
                lake_exit(5, 5, 48, 48, COLOR(1,1,1, 0.6)),
                lake_depth(373, 270, 48, 48),
                lake_buckit(427, 270, 48, 48),
                lake_throw(140, 252, 64, 64),
                lake_bait(320, 270, 48, 48),
    
                boatNo_button(335,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),
                boatYes_button(265,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),
    
                boatExit_button(410,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),

                catExit_button(410,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),
                catGive_button(340,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),
                busExit_button(410,275,60,45,COLOR(0.7f, 0.7f, 1.0f)),
                busGo_button(340, 275, 60, 45, COLOR(0.7f, 0.7f, 1.0f)),
                
                lake_LRBar(220, 280, LR_WIDTH, LR_HEIGHT),
                lake_DepthBar(SCREEN_WIDTH/2 - DepthDialogWidth/2,
                  SCREEN_HEIGHT/2 - DepthDialogHeigt/2,
                  DepthDialogWidth,
                  DepthDialogHeigt
                  ),
                BucketWindow( (SCREEN_WIDTH / 2) - 100,
                 (SCREEN_HEIGHT / 2) - 60,
                 200,
                 170
                 ),
                jrs0ul_button(SCREEN_WIDTH/2 - 82, 98, 164, 32),
                Depth_OK(lake_DepthBar.getX() + DepthDialogWidth/2 - 35,
                         lake_DepthBar.getY() + lake_DepthBar.height + 5,
                         70,
                         40,
                         COLOR(0.7, 0.7, 1.0f)),
                BucketWindow_OK (
                     (SCREEN_WIDTH / 2) - 35,
                     BucketWindow.getY()+BucketWindow.height - 42,
                     70, 40,
                     COLOR(0.7f, 0.7f, 1.0f)           
                     ),
                Bait_CfgWindow(SCREEN_WIDTH/2 - 130,
                   SCREEN_HEIGHT/2 - 100,
                   260,
                   200),
                Bait_Area(Bait_CfgWindow.getX() + 8,
                          Bait_CfgWindow.getY() + 20,
                          Bait_CfgWindow.width-2, 
                          12 + 3*BAITHEIGHT),
                Bait_OK(SCREEN_WIDTH/2 - 35,
                        Bait_Area.getY()+Bait_Area.height + 20,
                        70,
                        40,
                        COLOR(0.7f, 0.7f, 1.0f)),
                
                Play_Button(SCREEN_WIDTH/2 - 64,
                            130, 128, 30),
                Credits_Button(SCREEN_WIDTH/2 - 64,
                               250, 128, 30),
                Help_Button(SCREEN_WIDTH/2 - 64,
                            210, 128, 30),
    
                Options_Button(SCREEN_WIDTH/2 - 64,
                               170, 128, 30),

                QuitButton(SCREEN_WIDTH/2 - 64,
                           290, 128, 30),
    
                resetSaveGame_button(SCREEN_WIDTH/2 - 35, 200, 70, 40),
                linetension_button(70, 50, 48, 48),
                optionsOK_button(SCREEN_WIDTH/2 - 32, 270, 64, 30, COLOR(0.7f, 0.7f, 1.0f)),
                
                Shop_GoodsArea(170, 80, 320, 144),
                Rest_FishArea(175, 100, 320, 144),
                BusTicketArea(175, 138, 300, 100),
                
                yesno(40, 100, 400, 130),
                more_button(10, 250, 100, 64),
                prevHelpPage_button(5, 270, 48, 48),
                nextHelpPage_button(427, 270, 48, 48),
                helpBack_button(25, 15, 60, 30, COLOR(0.7f, 0.7f, 1.0f)),
                NoMusic_Button(SCREEN_WIDTH - 50, 2, 48, 50, COLOR(1,1,1,0.6f))
                
                                
    
                {

                    Scene V;

                    SceneRect SellPoint;
                    SellPoint.r.x = 280;
                    SellPoint.r.y = 40;
                    SellPoint.r.width = 95;
                    SellPoint.r.height = 100;
                    SellPoint.func = _enterSellPoint;
                    V.HotSpots.add(SellPoint);

                    //city
                    SceneRect City;
                    City.r.x = 170;
                    City.r.y = 50;
                    City.r.width = 100;
                    City.r.height = 60;
                    City.func = _enterCity;
                    V.HotSpots.add(City);

                    //baitshop
                    SceneRect BaitShop;
                    BaitShop.r.x = 100;
                    BaitShop.r.y = 40;
                    BaitShop.r.width = 70;
                    BaitShop.r.height = 100;
                    BaitShop.func = _enterBaitShop;
                    V.HotSpots.add(BaitShop);

                    SceneRect Lake;
                    Lake.r.x = 450;
                    Lake.r.y = 20;
                    Lake.r.width = 40;
                    Lake.r.height = 300;
                    Lake.func = _enterLakeSelect;
                    V.HotSpots.add(Lake);

                    SceneRect Home;
                    Home.r.x = 0;
                    Home.r.y =  20;
                    Home.r.width = 40;
                    Home.r.height = 300;
                    Home.func = _enterHome;
                    V.HotSpots.add(Home);

                    V.BgIndex = 31;

                    bool TownGrid[10][15] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,1,1,0,1,1,0,1,1,1,0,0,0},
                                             {0,0,0,1,1,0,1,1,0,1,1,1,0,0,0},
                                             {0,0,0,1,1,0,1,1,0,0,1,0,0,0,0},
                                             {0,0,0,1,0,0,1,1,0,0,1,0,0,0,0},
                                             {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

                    for (unsigned i = 0; i < 10; i++)
                        for (unsigned a = 0; a < 15; a++)
                            V.grid[i][a] = TownGrid[i][a];

                    V.useClouds = true;
                    V.ScaleFactor = 150;


                    Scenes.add(V);

                    Scene C;

                    SceneRect Town;
                    Home.r.x = 150;
                    Home.r.y =  200;
                    Home.r.width = 180;
                    Home.r.height = 60;
                    Home.func = _enterTown;
                    C.HotSpots.add(Home);


                    SceneRect Bus;
                    Bus.r.x = 360;
                    Bus.r.y =  10;
                    Bus.r.width = 180;
                    Bus.r.height = 65;
                    Bus.func = _enterBus;
                    C.HotSpots.add(Bus);




                    bool CityGrid[10][15] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
                                             {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0},
                                             {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
                                             {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
                                             {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                             {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

                    C.BgIndex = 32;
                    for (unsigned i = 0; i < 10; i++)
                        for (unsigned a = 0; a < 15; a++)
                            C.grid[i][a] = CityGrid[i][a];

                    C.useClouds = false;
                    C.ScaleFactor = 120;


                    Scenes.add(C);



                    Scene Atom;

                     bool AtomGrid[10][15] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
                                              {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
                                              {0,0,1,1,1,1,1,1,1,1,1,0,0,0,0},
                                              {0,0,1,1,1,1,1,1,1,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                              {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

                    Atom.BgIndex = 34;
                    for (unsigned i = 0; i < 10; i++)
                        for (unsigned a = 0; a < 15; a++)
                            Atom.grid[i][a] = AtomGrid[i][a];

                    Atom.useClouds = true;
                    Atom.ScaleFactor = 150;

                    Bus.r.x = 0;
                    Bus.r.y = 100;
                    Bus.r.width = 150;
                    Bus.r.height = 100;
                    Bus.func = _enterBus;
                    Atom.HotSpots.add(Bus);

                    Lake.r.x = 340;
                    Lake.r.y = 20;
                    Lake.r.width = 150;
                    Lake.r.height = 120;
                    Lake.func = _enterLakeSelect;
                    Atom.HotSpots.add(Lake);





                    Scenes.add(Atom);


                                   
                    
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
                    ScreenWidth = 480;
                    ScreenHeight = 320;
#else
                    ScreenWidth = 800;
                    ScreenHeight = 600;
                    windowed = false;
#endif

                clickCount = 0; GameState = INTRO; LineStartX = 183;
                LineStartY = MAXFISHY - 8;
                Ftim = 0;
                ZvejasFrame = 3;
                    
                fishingSpotsAnim = 0;
                fishingSpotAnimationDir = false;
                    
                strcpy(shopKeeperSays, "What would you like to buy?");
        
                MyRod.floatFrame = 0;
                MyRod.floatFrameTics = 0;
                //---
                lakeIndex = 0;
                lakes[0].bgIndex = 23;
                LakePoint p;
                p.point = Vector3D(30, 70, 0);
                p.type = 1;
                lakes[0].spots.add(p);

                //spot 1
                LakePoint p1;
                p1.point = Vector3D(150, 210, 0);
                p1.type = 0;
                FishPackElement fe;
                fe.kind = 0;
                fe.count = 7;
                p1.fes.fish.add(fe);
                fe.kind = 1;
                fe.count = 9;
                p1.fes.fish.add(fe);
                fe.kind = 2;
                fe.count = 3;
                p1.fes.fish.add(fe);
                fe.kind = 3;
                fe.count = 1;
                p1.fes.fish.add(fe);

                p1.func = 0;
                strncpy(p1.lakepics[1], "data/pics/shorensky2.tga", 255);
                strncpy(p1.lakepics[6], "", 255);

                lakes[0].spots.add(p1);
                //spot 2
                LakePoint p2;
                p2.point = Vector3D(150, 20, 0);
                p2.type = 0;
                fe.kind = 0;
                fe.count = 7;
                p2.fes.fish.add(fe);
                fe.kind = 1;
                fe.count = 9;
                p2.fes.fish.add(fe);
                fe.kind = 2;
                fe.count = 1;
                p2.fes.fish.add(fe);
                fe.kind = 3;
                fe.count = 3;
                p2.fes.fish.add(fe);
                p2.func = 0;
                strncpy(p2.lakepics[1], "data/pics/shorensky.tga", 255);
                strncpy(p2.lakepics[6], "", 255);
                lakes[0].spots.add(p2);
                //spot 3
                LakePoint p3;
                p3.point = Vector3D(390, 190, 0);
                p3.type = 0;
                fe.kind = 0;
                fe.count = 14;
                p3.fes.fish.add(fe);
                fe.kind = 2;
                fe.count = 6;
                p3.fes.fish.add(fe);
                p3.func = 0;
                strncpy(p3.lakepics[1], "data/pics/shorensky3.tga", 255);
                strncpy(p3.lakepics[6], "data/pics/forest.tga", 255);
                lakes[0].spots.add(p3);
                //spot 4
                LakePoint p4;
                p4.point = Vector3D(275, 265, 0);
                p4.type = 0;
                fe.kind = 1;
                fe.count = 5;
                p4.fes.fish.add(fe);
                fe.kind = 5;
                fe.count = 7;
                p4.fes.fish.add(fe);
                fe.kind = 4;
                fe.count = 7;
                p4.fes.fish.add(fe);
                fe.kind = 2;
                fe.count = 1;
                p4.fes.fish.add(fe);
                p4.func = 0;
                strncpy(p4.lakepics[1], "data/pics/shorensky4.tga", 255);
                strncpy(p4.lakepics[6], "", 255);
                lakes[0].spots.add(p4);
                //spot 5
                LakePoint p5;
                p5.point = Vector3D(320, 60, 0);
                p5.type = 0;
                fe.kind = 1;
                fe.count = 8;
                p5.fes.fish.add(fe);
                fe.kind = 4;
                fe.count = 9;
                p5.fes.fish.add(fe);
                fe.kind = 0;
                fe.count = 2;
                p5.fes.fish.add(fe);
                fe.kind = 2;
                fe.count = 1;
                p5.fes.fish.add(fe);
                p5.func = _enterBoat;
                strncpy(p5.lakepics[1], "data/pics/shorensky5.tga", 255);
                strncpy(p5.lakepics[6], "", 255);
                lakes[0].spots.add(p5);
                //spot 6
                LakePoint p6;
                p6.point = Vector3D(200, 90, 0);
                p6.type = 0;
                p6.locked = true;
                fe.kind = 2;
                fe.count = 11;
                p6.fes.fish.add(fe);
                fe.kind = 6;
                fe.count = 1;
                p6.fes.fish.add(fe);
                fe.kind = 3;
                fe.count = 5;
                p6.fes.fish.add(fe);
                fe.kind = 0;
                fe.count = 3;
                p6.func = 0;
                strncpy(p6.lakepics[1], "data/pics/shorensky6.tga", 255);
                strncpy(p6.lakepics[6], "", 255);
                p6.fes.fish.add(fe);

                lakes[0].spots.add(p6);

                LakePoint cat;
                cat.point = Vector3D(440, 100, 0);
                cat.type = 0;
                cat.locked = false;
                cat.func = _enterCat;
                lakes[0].spots.add(cat);
                
                


                //--
                lakes[1].bgIndex = 35;
                p.locked = false;
                p.point = Vector3D(30, 70, 0);
                p.type = 1;
                lakes[1].spots.add(p);
                
                LakePoint p7;
                p7.point = Vector3D(90, 180, 0);
                p7.type = 0;
                fe.kind = 2;
                fe.count = 12;
                p7.fes.fish.add(fe);
                fe.kind = 3;
                fe.count = 5;
                p7.fes.fish.add(fe);
                fe.kind = 0;
                fe.count = 3;
                p7.fes.fish.add(fe);
                p7.func = 0;
                strncpy(p7.lakepics[1], "data/pics/atomshore.tga", 255);
                strncpy(p7.lakepics[6], "", 255);

                lakes[1].spots.add(p7);


                //---
        
                MasalasCfg_Active = false;
                GylisCfg_Active = false;
                Buckit_Active = false;
                ExitDialog_Active = false;
                    
                launchjrs0ul = false;
                finishedPullOut = false;
                    
                islandEnabled = false;
                    
                Alpha1 = 1.0f;
                Alpha2 = 1.0f;
                WaterSwitch = false;
                throwAnimation = false;
                throwAnimationTic = 0;
                LineTension = 0;
                    
                strcpy(boatMan[0], boatManSays[0]);
                strcpy(boatMan[1], boatManSays[1]);
                strcpy(boatMan[2], boatManSays[2]);

                strncpy(busDriver[0], busDriverSays[0], 100);
                strncpy(busDriver[1], busDriverSays[1], 100);
                    
                strcpy(LakeGfxList[0], "data/pics/fishingtoolbar.tga");
                strcpy(LakeGfxList[1], "data/pics/shorensky.tga");
                strcpy(LakeGfxList[2], "data/pics/grass.tga");
                strcpy(LakeGfxList[3], "data/pics/lake.tga");
                strcpy(LakeGfxList[4], "data/pics/lake2.tga");
                strcpy(LakeGfxList[5], "data/pics/zv1.tga");
                strcpy(LakeGfxList[6], "");
                loadingLakePics = false;
                lakePicsLoaded = false;
                    
                fingerOnLR = false;
                launchmystore = false;
                
                tooMuchFishMessageActive = false;
                    
                oldMoveTouch = Vector3D(-1,-1,0);
                _bait = 0;
                loadingAnim = 0;

                Works = true;
                SceneIndex = 0;

                ticketIndex = 0;

                gotCatFishes = false;
               }
    
    void saveMyData();
    void init();
    void loadConfig();
    void logic();
    void render();
    void destroy();
    
};


#endif //_CPP_SINGLETON

