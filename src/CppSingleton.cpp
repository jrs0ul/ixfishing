
#include "CppSingleton.h"
#include "Collisions.h"


//-------------------------------
void Singleton::saveMyData(){
    char buf[255];
    sprintf(buf, "%s/game.sav", DocumentPath);
    me.save(buf);
}

//------------------------------
void Singleton::loadlang(){
    
    char* data = 0;
    if (!ReadFileData(lang_def, &data)){
        printf("failed to read lang file [%s]\n", lang_def);
        return;
    }
    
    char * s = 0;
    s = strtok(data, "\r\n");
    puts(s);
    unsigned index = 1;
    
    while ((s)&&(index < 24)){
        if (s){
            strcpy(lang[index], s);
        }
        index++;
        s = strtok(0, "\r\n");
    }
    
    free(data);
}

//---------------------------------------------
void Singleton::draw_intro(){
    pics.draw(22, 0, 0, false, 0, 480/512.0f, 320/256.0f, 0);
    pics.draw(14, SCREEN_WIDTH/2 - 256/2,
              0, 0, false, 1.0f, 1.0f, 0,
              COLOR(1.0f, 1.0f, 1.0f, 1.0f),
              COLOR(1.0f, 1.0f, 1.0f, 0.8f));
    
    
//#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    more_button.draw(pics, 25, 0);
//#endif
    Play_Button.draw(pics, 18, 0);
    Help_Button.draw(pics, 18, 2);
    Credits_Button.draw(pics, 18, 1);
    Options_Button.draw(pics, 18, 3);
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    QuitButton.draw(pics, 18, 4);
#endif
}
//------------------------------------
void Singleton::drawOptions(){
    draw_intro();
    int x = SCREEN_WIDTH/2 - 180;
    int y = 25;
    int width = 360;
    int height = 280;
    COLOR c1 = COLOR(0.5f,0.0f,0.8f, 0.9f);
    COLOR c2 = COLOR(0.2f,0.0f,0.3f, 0.7f);
    pics.draw(-1, x, y, 0, false, width, height, 0, 
              c1, c2);
    
    pics.draw(8, x - 32, y, 0, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    pics.draw(8, x + width, y, 1, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    
    pics.draw(18, SCREEN_WIDTH/2, 32, 3, true);
    resetSaveGame_button.drawTextnPicture(pics, 7, 2, 0, "Reset");
    optionsOK_button.drawTextnPicture(pics, 7, 2, 0, "OK");
    linetension_button.draw(pics, 2, (!linetension_button.state) ? 10 : 11);
    WriteShadedText(130, 65, pics, 0, "Does line tension exists?");
    WriteShadedText(130, 110, pics, 0, "Music volume:");
    WriteShadedText(50, 180, pics, 0, "Stuck? Want to start everything all over?");
    
    musicVolumeBar.draw(pics, 2, 8, 9);
    if (yesno.active())
        yesno.draw(pics, 0, 8, 2, 7, 128.0f);
}

//-----------------------------------
void Singleton::optionsLogic(){
    
    if (touches.move.count()){
        musicVolumeBar.processInput(touches.move[0].v[0], touches.move[0].v[1]);
        sys.musicVolume = musicVolumeBar.state()/100.0f; 
        music.setVolume(sys.musicVolume);
    }
    if (touches.down.count()){
        musicVolumeBar.processInput(touches.down[0].v[0], touches.down[0].v[1]);
        sys.musicVolume = musicVolumeBar.state()/100.0f; 
        music.setVolume(sys.musicVolume);
    }
    
    if (touches.up.count()){
        int mx = touches.up[0].v[0];
        int my = touches.up[0].v[1];
        
        
        if (linetension_button.isPointerOnTop(mx, my)){
            linetension_button.state = (!linetension_button.state)?1:0;
            sys.useLineTension = linetension_button.state;//)?0:1;
        }
        
        if (musicVolumeBar.isPointerOnNextpage(mx, my)){
            musicVolumeBar.pageDown();
            sys.musicVolume = musicVolumeBar.state()/100.0f; 
            music.setVolume(sys.musicVolume);
            //ss.playsound(8);
        }
        if (musicVolumeBar.isPointerOnPrevpage(mx, my)){
            musicVolumeBar.pageUp();
            sys.musicVolume = musicVolumeBar.state()/100.0f; 
            music.setVolume(sys.musicVolume);
            //ss.playsound(8);
        }
        
        if (yesno.active()){
            
            if (yesno.isPointerOnYes(mx, my)){
                char buf[255];
                sprintf(buf, "%s/game.sav", DocumentPath);
                remove(buf);
                me.reset();
                Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                masalas = 0;
                krepsis_def = 0;
                yesno.deactivate();
            }
            if (yesno.isPointerOnNo(mx, my)){
                yesno.deactivate();
            }
            return;
        }
        
        if (resetSaveGame_button.isPointerOnTop(mx, my)){
            yesno.activate();
            
        }
        
        if (optionsOK_button.isPointerOnTop(mx, my)){
            GameState = INTRO;
            char buf[255];
            sprintf(buf, "%s/settings.cfg", DocumentPath);
            sys.write(buf);
        }
    }
}



//__________________________________________
void Singleton::LoadFiles(){
    
    puts("Loading - please wait ");
    
    char dir[128];

    strcpy(dir, "data/pics/");

    
    Font.load(pics, 0, "charai.tga", dir); 
    pics.loadFile("cloud.tga", 1, 64, 32, dir, 1);
    pics.loadFile("buttons.tga", 2, 48, 48, dir, 1);
    pics.loadFile("baits.tga", 3, 48, 48, dir, 1);
    pics.loadFile("mzg.tga", 4, 16, 16, dir, 1);
    pics.loadFile("panel.tga",5, 512, 64, dir, 1);
    pics.loadFile("townpanel.tga", 6, 480, 102, dir, 0);
    pics.loadFile("dude.tga", 7, 64, 128, dir, 0);
    pics.loadFile("guistuff.tga",8, 64, 128, dir, 1);
    pics.loadFile("puzis.tga", 9, 512, 256, dir, 1);
    pics.loadFile("ranka.tga", 10, 128, 64, dir, 0);
    pics.loadFile("eser0.tga", 11, 64, 123, dir, 1);
    pics.loadFile("kuoj0.tga", 12, 64, 154, dir, 1);
    pics.loadFile("pike0.tga", 13, 64, 256, dir, 1);
    pics.loadFile("title.tga", 14, 256, 128, dir, 1);
    pics.loadFile("tcloud.tga", 15, 137, 64, dir, 1);
    pics.loadFile("plude.tga", 16, 8, 16, dir, 1);
    pics.loadFile("npc.tga", 17, 512, 256, dir, 1);
    pics.loadFile("menu.tga", 18, 128, 64, dir, 1);
    pics.loadFile("rodykle.tga", 19, 32, 32, dir, 1);
    pics.loadFile("metterbar.tga", 20, 16, 16, dir, 1);
    pics.loadFile("guistuff2.tga", 21, 128, 64, dir, 1);
    pics.loadFile("titlebg.tga", 22, 512, 256, dir, 1);
    pics.loadFile("biglake.tga", 23, 512, 256, dir, 1);
    pics.loadFile("storkis.tga", 24, 64, 192, dir, 1);
    pics.loadFile("moregames.tga",25, 100, 64, dir, 1);
    pics.loadFile("markers.tga", 26, 8, 8, dir, 0);
    pics.loadFile("pugzlys.tga", 27, 57, 122, dir, 1);
    pics.loadFile("karsis.tga",28, 113, 230, dir, 1);
    pics.loadFile("captainlol.tga",29, 512, 256, dir, 1);
    pics.loadFile("balionas.tga", 30, 128, 64, dir, 1);
    pics.loadFile("town.tga", 31, 480, 256, dir, 1);
    pics.loadFile("city.tga", 32, 480, 256, dir, 1);
    pics.loadFile("vitoldik.tga", 33, 480, 320, dir, 1);
    pics.loadFile("atom.tga", 34, 480, 256, dir, 1);
    pics.loadFile("atomlake.tga", 35, 512, 256, dir, 1);
    pics.loadFile("thecat.tga", 36, 512, 256, dir, 1);
    pics.loadFile("catfish.tga", 37, 64, 403, dir, 1);
  
    puts("images +/- loaded ;)");
    loadlang();
    puts("languages loaded!");
}

//--------------------------------------
void Singleton::loadConfig(){

    char buf[255];
    sprintf(buf, "%s/settings.cfg", DocumentPath);
    sys.load(buf);
    ScreenWidth = sys.ScreenWidth;
    ScreenHeight = sys.ScreenHeight;
    windowed = sys.useWindowed;

    sys.write(buf);

}
//----------------------------------------
void Singleton::init(){
    strcpy(lang_def, "data/lang/eng.dat");
        
    LoadFiles();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    fishData.load("data/fishData.xml");
    
    baitData.load("data/baits.xml");
    char buf[255];
    sprintf(buf, "%s/game.sav", DocumentPath);
    if (!me.load(buf))
        me.addInitialBaits();
    else{
        krepsis_def = me.totalCatchWeight();
    }
    
    
    linetension_button.state = sys.useLineTension;//)? 0 : 1;
    musicVolumeBar.set(80, 130, 2, 100, sys.musicVolume*100, 2);
    //FxVolume.set(80, 200, 2, 100, sys.fxVolume*100, 2);
    
    
    MyRod.depth = 10;
    masalas=0;
    
    ss.init(0);
    ss.loadFiles("data/sfx/", "audio.txt");
    music.open("data/music.ogg");
    music.setVolume(sys.musicVolume);
    music.playback();
   
    useAccel = false;
    
    
}

//--------------------
void Singleton::showstatus(int x, int y, unsigned type){
    char buf[255];
    
    switch(type){
        case 1:{
            
            sprintf(buf,"%d / %d g", krepsis_def, krepsis_max );
            WriteShadedText(x, y + 8, pics, 0, buf);
            sprintf(buf,"%.2f $", me.money() );
            WriteShadedText(x, y + 48, pics, 0, buf);
            
            
            int posX = 145;
            unsigned count = (fishData.count() < 4)? fishData.count() : 4;
            for (unsigned i = 0; i < count; i++){
                sprintf(buf,"%7s : %3d ", fishData.getType(i)->name, me.catchCountByKind(i));//pike
                WriteShadedText(x + posX, y + i*16, pics, 0, buf);
            }
            if (fishData.count() > 4){
                posX = 280;
                count = fishData.count();
                for (unsigned i = 4; i < count; i++){
                    sprintf(buf,"%7s : %3d ", fishData.getType(i)->name, me.catchCountByKind(i));//pike
                    WriteShadedText(x + posX, y + (i-4)*16, pics, 0, buf);
                }   
            }
            unsigned long total = me.catchCount();
            //if (total){
                sprintf(buf,"%7s : %3lu ", lang[20], total );
                Font.setcolor(0,0,0);
                Font.display(x + posX + 1, y + 65, buf);
                Font.setcolor(1, 1, 1);
                Font.display(x + posX, y+64, buf);
            //}
            
        } break;
        default:{
            Font.setcolor(0,0,0);
            sprintf(buf," Weight: %d / %d g", krepsis_def, krepsis_max );
            Font.display(x, y, buf);
            int posX = -25;
            int posY = 18;
            unsigned count = (fishData.count() < 4)? fishData.count() : 4;
            for (unsigned i = 0; i < count; i++){
                sprintf(buf,"%7s : %3d ", fishData.getType(i)->name, me.catchCountByKind(i));//pike
                WriteText(x + posX, y + posY+ i*15, pics, 0, buf, 1,1, COLOR(0,0,0), COLOR(0,0,0));
            }
            if (fishData.count() > 4){
                posX = 100;
                count = fishData.count();
                for (unsigned i = 4; i < count; i++){
                    sprintf(buf,"%7s : %3d ", fishData.getType(i)->name, me.catchCountByKind(i));//pike
                    WriteText(x + posX, y + posY + (i-4)*15, pics, 0, buf, 1,1, COLOR(0,0,0), COLOR(0,0,0));
                }   
            }
            unsigned long total = me.catchCount();
            //if (total){
            sprintf(buf,"%7s : %3lu ", lang[20], total );
            Font.setcolor(0,0,0);
            Font.display(x + posX + 1, y +posY+ 55, buf);
           
            //}
            
            sprintf(buf,"%s : %.2f $ ", lang[5], me.money() );
            Font.display(x+10,y+96, buf);
            Font.setcolor(255, 255, 255);
        }
    }
}
//---------------------------
void Singleton::GenerateClouds(int horizon){
    for (int i = 0; i < CLOUDCOUNT; i++){
        Clouds[i*4] = rand() % 480;
        Clouds[i*4 + 1] = rand() % horizon - 16;
        Clouds[i*4 + 2] = rand() % 5 + 2;
        Clouds[i*4 + 3] = 0;
        
    }
}
//-----------------------
void Singleton::DrawClouds(){
    for (int i = 0; i < CLOUDCOUNT; i ++){
        pics.draw(1, Clouds[i*4], Clouds[i*4 + 1], 0);
    }
}

//--------------------
void Singleton::DrawAdventureMode(unsigned index, bool debug){

    Scene * sc = &Scenes[index];

    pics.draw(sc->BgIndex, 0, 0);
    if (sc->useClouds)
        DrawClouds();
    NoMusic_Button.draw(pics, 2, 5 + NoMusic_Button.state);
   
    pics.draw(6, 0, 223);
    showstatus(60, 240, 1);
    if (debug){
        for (unsigned i = 0; i < sc->HotSpots.count(); i++){
            xbox(pics, 
                 sc->HotSpots[i].r.x,  sc->HotSpots[i].r.y,
                sc->HotSpots[i].r.x + sc->HotSpots[i].r.width,
                sc->HotSpots[i].r.y + sc->HotSpots[i].r.height, COLOR(255, 0, 0));
        }

    

        for (unsigned i = 0; i < 10; i++){
            for (unsigned a = 0; a < 15; a++){
                pics.draw(-1, a * 32, i * 32, 0, false, 32, 32, 0,
                          (!sc->grid[i][a])?COLOR(1,0,0,0.5f):COLOR(0,0,0,0),
                          (!sc->grid[i][a])?COLOR(1,0,0,0.5f):COLOR(0,0,0,0));
            }
        }

        for (unsigned i = 0; i < walkPath.parent.count(); i++){
    
            pics.draw(-1, walkPath.parent[i].x * 32, 
                          walkPath.parent[i].y * 32,
                          0, false, 32, 32, 0,
                          COLOR(0,0,1,0.5),
                        COLOR(0,0,1,0.5));
            char buf[50];
            sprintf(buf, "%u", i);
            WriteShadedText(walkPath.parent[i].x * 32, walkPath.parent[i].y * 32,
                            pics, 0, buf);

        }
    }
    
    
    pics.draw(7, Dude.x , Dude.y, Dude.frame, false, 
              Dude.scale,
              Dude.scale);
    if (debug)
        xbox(pics, Dude.x, Dude.y + (50 * Dude.scale), 
             Dude.x + 64 * Dude.scale,
            Dude.y + (128 * Dude.scale), COLOR(255, 0, 0));
    
   
}
//---------------------
void Singleton::mainMenuLogic(){
   
    if ((OldKeys[6])&&(!Keys[6])){
        saveMyData();
        Works = false;
    }
        

    if (touches.up.count()){
        
        if (Play_Button.pressed){
            Dude.x = 50;
            Dude.y = 150;
            TikslasX = Dude.x;
            TikslasY = Dude.y;
            GameState = TOWN;
            GenerateClouds(40);
            Play_Button.reset();
        }
        if (Credits_Button.pressed){
            GameState = CREDITS;
            Credits_Button.reset();
        }
        if (Help_Button.pressed){
            GameState = HELP;
            activeHelpPage = 0;
            Help_Button.reset();
        }
        
        if (Options_Button.pressed){
            GameState = OPTIONS;
            Options_Button.reset();
        }

        if (QuitButton.pressed){
            saveMyData();
            Works = false;
        }
        
//#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        if (more_button.pressed){
            more_button.c = COLOR(1,1,1);
            launchmystore = true;
            more_button.reset();
        }
//#endif
    }
    
    if (touches.down.count()){
        int mx = touches.down[0].v[0];
        int my = touches.down[0].v[1];
        if (Play_Button.isPointerOnTop(mx, my)){
            Play_Button.scalex = 1.5f;
            Play_Button.pressed = true;
        }
        if (Credits_Button.isPointerOnTop(mx, my)){
            Credits_Button.scalex = 1.5f;
            Credits_Button.pressed =true;
        }
        if (Help_Button.isPointerOnTop(mx, my)){
            Help_Button.scalex = 1.5f;
            Help_Button.pressed = true;
        }
        if (Options_Button.isPointerOnTop(mx, my)){
            Options_Button.scalex = 1.5f;
            Options_Button.pressed = true;
        }

        if (QuitButton.isPointerOnTop(mx, my)){
            QuitButton.scalex = 1.5f;
            QuitButton.pressed = true;
        }
        
        if (more_button.isPointerOnTop(mx, my)){
            more_button.c = COLOR(0.5f, 0.5f, 0.5f);
            more_button.pressed = true;
        }
    }
}
//----------------------
void Singleton::MoveClouds(int horizon){
    for (int i = 0; i < CLOUDCOUNT; i++){
        (Clouds[i*4 + 3])++;
        if (Clouds[i*4 + 3] > Clouds[i*4 + 2]){
            Clouds[i*4] -= 1;
            Clouds[i*4 + 3] = 0; 
            if (Clouds[i*4] < -64){
                Clouds[i*4] = 500;
                Clouds[i*4 + 1] = rand() % horizon - 16;
                Clouds[i*4 + 2] = rand() % 4 + 2;
                Clouds[i*4 + 3] = 0;
            }
        }
    }
}


//----------------------
void Singleton::Randomfish(FishPack& pack){
    fishes.destroy();
    zuvID = 0;
    for (unsigned i = 0; i < pack.fish.count(); i++){
        
        for (int a = 0; a < pack.fish[i].count; a++){
            Fish z;
        
            z.dead = false;
            z.x = rand() % (MAXFISHX - MINFISHX) + 1;
            z.y = rand() % MAXPOWER + 1;
        
            z.kind = pack.fish[i].kind;
       
            printf("kind: %d\n", z.kind);
        
            FishType * ft = fishData.getType(z.kind);
        
            z.depth =rand() % ft->mindepth + 1;
            z.mase = rand() % ft->maxweight 
                  + ft->minweight;
            
            z.dir = 0;
            z.tim = 0;
            
            fishes.add(z);
        }
    }
}
//---------------------
void Singleton::enterBus(){

    GameState = BUS;
    busDriverSpeachIndex = 0;
    if (SceneIndex == 2){
        Dude.x = 360;
        Dude.y = 70;
    }
    else{
        Dude.x = 170;
        Dude.y = 150;

    }

}

//---------------------
void Singleton::enterSellPoint(){

    GameState = SELLPOINT;
    Dude.x = 260;
    Dude.y = 150;
}

//---------------------
void Singleton::enterCity(){
    SceneIndex = 1;
    Dude.x = 190;
    Dude.y = 120;

}
//--------------------
void Singleton::enterTown(){
    SceneIndex = 0;
    Dude.x = 195;
    Dude.y = 95;
    Dude.scale = 0.37f;
    makePath(0, 190, 150);

}


//--------------
void Singleton::enterBaitShop(){
    GameState = STORE;
    Dude.x = 97;
    Dude.y = 150;
}
//--------------
void Singleton::enterLakeSelect(){
    GenerateClouds(30);
    GameState = LAKESELECT;
    thrownPoint = Vector3D(190, 100, 0);
        
    if (SceneIndex == 0){
        Dude.x = 410;
        Dude.y = 150;
    }
    else{
        Dude.x = 300;
        Dude.y = 130;
    }

}
//-------------
void Singleton::enterHome(){
    GameState = INTRO;
}
//----------------
void Singleton::makePath(unsigned sceneIndex, int x, int y){

    _Point dst;
    dst.x = x / 32;
    dst.y = y / 32;

    while ((!Scenes[sceneIndex].grid[dst.y][dst.x])&&(dst.y < 10))
            dst.y++;


    walkPath.destroy();
    pathStep = 0;
    _Point src;
    src.x = (int)((Dude.x + (64 * Dude.scale)/2)/32);
    src.y = (int)((Dude.y + 128 * Dude.scale)/32);

    bool ** bmap = 0;

    bmap = (bool **)malloc( 10 * sizeof(bool*));
    for (unsigned i = 0; i < 10; i++){
        bmap[i] = (bool*)malloc(15 * sizeof(bool));
        for (unsigned a = 0; a < 15; a++){
            bmap[i][a] = Scenes[sceneIndex].grid[i][a];
        }
    }

    walkPath.findPath(src, dst, bmap, 15, 10);

    if (bmap){
        for (unsigned i = 0; i < 10; i++)
            free(bmap[i]);
        free(bmap);
        bmap = 0;
    }

}



//----------------------
void Singleton::AdventureLogic(){
   
    if ((OldKeys[6])&&(!Keys[6]))
        GameState = INTRO;


    if (touches.oldDown.count()){
        if (NoMusic_Button.isPointerOnTop(MouseX, MouseY)){
            if (!NoMusic_Button.state){
                music.stop();
                NoMusic_Button.state = 1;
            }
            else{
                music.playback();
                NoMusic_Button.state = 0;
            }
            return;
        }
            }
       
    
    MoveClouds(40);
    if ((touches.down.count()) && (MouseY < 230)){

        makePath(SceneIndex, MouseX, MouseY);
    }
    
    Dude.tic++;
    int speedX = 5;
    Dude.scale = 0.5f - ((Scenes[SceneIndex].ScaleFactor - Dude.y)/300.0f);
    
    if (Dude.tic == 8){

        if (walkPath.parent.count()){
            TikslasX = walkPath.parent[pathStep].x * 32;
            TikslasY = walkPath.parent[pathStep].y * 32 - (78 * Dude.scale);
        
            if (!(( Dude.x > TikslasX - speedX) && (Dude.x < TikslasX + speedX))){
    
                if (TikslasX - speedX < Dude.x){
                    Dude.x -= speedX;
                    Dude.frame = (Dude.frame == 0)? 1 : 0;
                }
                else if (TikslasX + speedX > Dude.x){
                    Dude.x +=  speedX;
                    Dude.frame = (Dude.frame == 3)? 2 : 3;
                }
            }
            else{
    
                if (!(( Dude.y > TikslasY - 2) && (Dude.y < TikslasY + 2))){

                    if (TikslasY > Dude.y){
                        Dude.y +=  2;
                        if ((Dude.frame >= 6)&&(Dude.frame < 8))
                            Dude.frame = (Dude.frame == 7)? 6 : 7;
                        else{
                            Dude.frame = 6;
                        }
                    }
                    else if (TikslasY < Dude.y){
                            Dude.y -=  2;
                        if ((Dude.frame >= 4)&&(Dude.frame < 6))
                            Dude.frame = (Dude.frame == 5)? 4 : 5;
                        else{
                            Dude.frame = 4;
                        }
                    }
                }

                else{
                    if (pathStep < walkPath.parent.count() - 1){
                        pathStep++;
                    }

                }
            }
            
        }
        Dude.tic = 0;
    }

    Rectangle_ dud(Dude.x, Dude.y + (50 * Dude.scale),
                  64 * Dude.scale, 78 * Dude.scale);

    for (unsigned i = 0; i < Scenes[SceneIndex].HotSpots.count(); i++){
        if (RectanglesColide(dud, Scenes[SceneIndex].HotSpots[i].r)) {
            walkPath.destroy();
            (*Scenes[SceneIndex].HotSpots[i].func)(this);
            return;
        }
    }

}
//--------------------
COLOR Singleton::MapDepthColor(int depth){
    if (depth > 0){
        if (depth < 21)
            return COLOR(0, 0, 1.0f);
        if (depth < 41)
            return COLOR(0, 1.0f, 0);
        return COLOR(0, 1.0f, 1.0f);
    }
    return COLOR(0, 0, 0);
}
//----------------------
void Singleton::DrawLine(float x1, float y1, float x2, float y2){
    
    float * vertices = (float*)malloc(sizeof(float)*4);
    float * colors = (float*)malloc(sizeof(float)*8);
    vertices[0] = x1;
    vertices[1] = y1;
    vertices[2] = x2;
    vertices[3] = y2;
    
    colors[0] = 1.0f;
    colors[1] = 1.0f;
    colors[2] = 1.0f;
    colors[3] = 1.0f;
    colors[4] = 1.0f;
    colors[5] = 1.0f;
    colors[6] = 1.0f;
    colors[7] = 1.0f;
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_FLOAT, 0, colors);
    
    glDrawArrays(GL_LINES, 0, 2 );
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    
}
//-----------------------------------
void Singleton::xbox(PicsContainer& pics, int x, int y, int x1, int y1, COLOR c){
    pics.draw(-1, x, y, 0, false, x1 - x, 1, 0, c, c);
    pics.draw(-1, x, y, 0, false, 1, y1 - y, 0, c, c);
    pics.draw(-1, x, y1, 0, false, x1 - x, 1, 0, c, c);
    pics.draw(-1, x1, y, 0, false, 1, y1 - y, 0, c, c);
    
}
//------------------------------------
void Singleton::drawFishXY(int x, int y){
    
    int scaleX = 3;
    int scaleY = 3;
    
       if (uzmesta)
        pics.draw(26, x + (fishx / scaleX), 
                  y + (fishy / scaleY),
                  1, true,
                  1.0f, 1.0f, 0.0f, COLOR(1.0f, 1.0f, 1.0f), COLOR(1.0f, 1.0f, 1.0f));
    for (unsigned i = 0; i < fishes.count(); i++){
        Fish * z = &fishes[i];
        
        if (!z->dead){
            COLOR color = MapDepthColor(z->depth);
            pics.draw(26, x + (z->x / scaleX),
                      y + (z->y / scaleY), 0, true,
                      1.0f,
                      1.0f,
                      0.0f,
                      color, color);
            
        }
    }
}
//-------------------------
void Singleton::DrawLRBar(int x, int y){
    WriteShadedText(x, y - 16, pics, 0, lang[22]);
    pics.draw(-1, x, y, 0, false, LR_WIDTH, LR_HEIGHT, 0,
              COLOR(0.4f, 0.4f, 0.4f), COLOR(0.08f, 0.08f, 0.08f));
    int halfField = (MAXFISHX - MINFISHX) / 2;
    float bar_percent = (LR_WIDTH/100.0f);
    float field_percent = (MAXFISHX - MINFISHX)/100.0f;
    
    pics.draw(2, x+ (((lr + halfField)/ field_percent)*bar_percent), 
                 y + LR_HEIGHT/2, 0, true, 6/48.0f, LR_HEIGHT/48.0f, 0);
}
//--------------------------------
void Singleton::DrawPowerBar(int x, int y,
                  COLOR color){
    Font.setcolor(0, 0, 0);
    Font.display(x - 5 * 11 + 1, y + 1, lang[21]);
    Font.setcolor(255, 255, 255);
    Font.display(x - 5 * 11, y, lang[21]);
    pics.draw(-1, x, y, 0, false, power, 16, 0, color, color);
    //frame3d(pics, x - 1, y - 1, x + 1 + MAXPOWER, y + 12, false);
    DrawCoolMetterFrame(x, y, MAXPOWER, 16);
    
}
//-------------------------------
void Singleton::DrawTensionBar(int x, int y){
    Font.setcolor(0, 0, 0);
    COLOR begining(0.0f, 1.0f, 0.0f);
    COLOR middle  (1.0f, 1.0f, 0.0f);
    COLOR end     (1.0f, 0.0f, 0.0f);
    WriteShadedText(x - 115, y, pics, 0, "Line Tension");
    float halftension = LineTensionMax/2.0f;
    if (LineTension < halftension){
        float k = LineTension/halftension;
        pics.draw(-1, x, y, 0, false, LineTension, 16, 0, begining, 
                  COLOR(((begining.c[0])? begining.c[0] - k : 0) + middle.c[0]*k,
                        ((begining.c[1])? begining.c[1] - k : 0) + middle.c[1]*k,
                        ((begining.c[2])? begining.c[2] - k : 0) + middle.c[2]*k), true);
    }
    else {
        pics.draw(-1, x, y, 0, false, halftension, 16, 0, begining, middle, true);
    }
    if (LineTension > halftension){
        float k = ((LineTension - halftension)*1.0f)/halftension;
        printf("%f\n",k);
        pics.draw(-1, x+halftension, y, 0, false, LineTension - halftension, 16, 0, middle,
                  COLOR(((middle.c[0])? middle.c[0] - k : 0) + end.c[0]*k,
                        ((middle.c[1])? middle.c[1] - k : 0) + end.c[1]*k,
                        ((middle.c[2])? middle.c[2] - k : 0) + end.c[2]*k), true);
    }

    DrawCoolMetterFrame(x, y, LineTensionMax, 16);
}

//------------------------------------
void Singleton::DrawCoolMetterFrame(int x, int y, unsigned width, unsigned height){
    pics.draw(20, x-7, y, 0, false, 1, height/16.0f);
    pics.draw(20, x+9, y, 1, false, (width-18)/16.0f, height/16.0f);
    pics.draw(20, x+width-9, y, 2, false, 1, height/16.0f);
}


//---------------------------------
void Singleton::DrawWindBar(int x, int y){
    
    //draw wind bar
    //frame3d(pics, x-1,y-1,x + 48,y+12,false);
    
    unsigned height = 16;
    unsigned width = 48;
    
    COLOR windColor1(0.1f, 0.1f, 0.2f);
    COLOR windColor2(0.2f, 0.5f, 0.99f);
    
    pics.draw(-1, x, y, 0, false, width, height, 0, windColor1, windColor1);
    pics.draw(-1, x + width/2, y, 0, false, wind, height, 0, windColor2, windColor2);
    
    DrawCoolMetterFrame(x, y, width, height);
    
    Font.setcolor(0, 0, 0);
    Font.display(x - 11 * 4 + 1-4, y + 3, lang[23]);
    Font.setcolor(255, 255, 255);
    Font.display(x - 11 * 4 - 4, y+2, lang[23]);
    
}
//------------------------------------
void Singleton::DrawHandnFish(int x, int y){
    
    pics.draw(10, x, y, 0);
    FishType * ft = fishData.getType(fishes[zuvID].kind);
    unsigned picID = ft->picture;
    int averageweight = (ft->maxweight - ft->minweight)/2 + ft->minweight;
    float aspect =  (fishes[zuvID].mase * 1.0f) /
    (averageweight * 1.0f);
    int _x = (aspect < 1.0)? (x + 75 + 10):(x + 75);
    int _y = y;
    if (fishes[zuvID].kind == 2){ //ugly hack
        _x -= 15;
        _y -= 15;
    }
    pics.draw(11+picID, _x, _y, 0, false, aspect, aspect);
}
//----------------------------------
void Singleton::DrawBaitCfg(){
    Font.setcolor(0, 0, 0);
    
    pics.draw(-1, Bait_CfgWindow.getX()-15,
              Bait_CfgWindow.getY(),
              0, false,
              Bait_CfgWindow.width+30 ,
              Bait_CfgWindow.height + 30,
              0,
              PANELCOLOR, PANELCOLOR);
    
    
    
    char buf[255];
    for (unsigned i = 0; i < me.baitCount(); i++){
       
        int y = Bait_Area.getY() + (i * BAITHEIGHT);
        
        if ((y > 30)&&(y < 250)){
            pics.draw(3, Bait_Area.getX()-5, y,  me.getBait(i)->index, false);
            TBait * b = baitData.getBait(me.getBait(i)->index);
            sprintf(buf,"%13s %3d %2s", b->name, me.getBait(i)->count, b->units);
            Font.display(Bait_Area.getX() + BAITHEIGHT, 
                         y  + BAITHEIGHT/2, buf);
            
            
        }
    }
    
    if ((_bait > -1)&&(masalas)&&
         (Bait_Area.getY() + (_bait)*BAITHEIGHT > 30)&&
         (Bait_Area.getY() + (_bait)*BAITHEIGHT < 250))
        xbox(pics, Bait_Area.getX()-5, Bait_Area.getY() + _bait*BAITHEIGHT,
             Bait_Area.getX() + 245,
             Bait_Area.getY() + (_bait+1) * BAITHEIGHT,
             COLOR(1.0f, 0, 0));
    
    
    pics.draw(-1, Bait_CfgWindow.getX() + Bait_CfgWindow.width - 2, Bait_CfgWindow.getY()+35, 0, 0, 5, 150, 0, COLOR(0.1f, 0.1f, 0.1f, 0.7f), COLOR(0.1f, 0.1f, 0.1f, 0.7f));
    float barheight = (me.baitCount()<= 3)?150.0f : (150.0f/100.0f) * (3.0f/(me.baitCount()/100.0f));
    //printf("bait area: %d\n", Bait_Area.getY());
    float barY = Bait_CfgWindow.getY()+35+(80-Bait_Area.getY())/(((me.baitCount())*48.0f)/150.0f);
    pics.draw(-1, Bait_CfgWindow.getX() + Bait_CfgWindow.width - 2,
              
              barY,
              
              0, 0, 5, barheight, 0, 
              COLOR(0.7f, 0.7f, 0.7f, 0.6f), COLOR(0.7f, 0.7f, 0.7f, 0.6f));
    
    COLOR P(0.66f, 0.66f, 0.66f, 1.0f);
    
    
    pics.draw(21, Bait_CfgWindow.getX() + Bait_CfgWindow.width/2, Bait_CfgWindow.getY(), 0, true,
              (Bait_CfgWindow.width+30)/128.0f, 60/64.0f, 0, P, P);
    
    WriteShadedText(SCREEN_WIDTH/2 - 50, Bait_CfgWindow.getY()-20, pics, 0, lang[16], 1.8f, 1.8f, COLOR(1,1,1), COLOR(1,1,1),COLOR(0,0,0));
    
    pics.draw(21, Bait_CfgWindow.getX()+ Bait_CfgWindow.width/2, Bait_CfgWindow.getY()+ Bait_CfgWindow.height + 20, 1, true,
              (Bait_CfgWindow.width+30)/128.0f, 60/64.0f, 0, P, P);
    
    Bait_OK.drawTextnPicture(pics, 7, 2, 0, lang[15]);
    Font.setcolor(255, 255, 255);
    
   
}

//___________________________________________
void Singleton::baitCfgLogic(){
    
    if (!uzmesta){
        
        
        if (touches.move.count()){
            Vector3D m = touches.move[0];
            if (oldMoveTouch.v[0] > -1){
                 Vector3D diff = m - oldMoveTouch;
                
                int minY = 180-48*(me.baitCount()-1);
                if ((Bait_Area.getY()+ diff.v[1] < 80)&&(Bait_Area.getY()+ diff.v[1] > minY))
                    Bait_Area.setpos(Bait_Area.getX(), Bait_Area.getY() + diff.v[1]);
            }
            
            oldMoveTouch = touches.move[0];
        }
        
        
        if ((touches.oldDown.count())&&(!touches.down.count())){
            
            Bait_Area.height = me.baitCount()*48;
            if (Bait_Area.isPointerOnTop(MouseX, MouseY)&&(MouseY < 240)&&(MouseY > 40)){
                int _y = MouseY - Bait_Area.getY();
                _bait = _y / BAITHEIGHT;
                
                if (_bait >= (int)me.baitCount())
                    _bait = me.baitCount() - 1;
                
                
                if (me.getBait(_bait)->count > 0)
                    masalas = me.getBait(_bait)->index;
            }
            
        }
        
        if (touches.up.count()){
            int mx = touches.up[0].v[0];
            int my = touches.up[0].v[1];
            
            if (Bait_OK.isPointerOnTop(mx, my)){
                MasalasCfg_Active = false;
                if (masalasold != masalas){
                    me.getBait(_bait)->count -= baitData.getBait(me.getBait(_bait)->index)->usedCount;
                    if (me.getBait(_bait)->count <= 0){
                        
                        me.removeBait(_bait);
                        Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                    }
                }
            }
            
            oldMoveTouch = Vector3D(-1,-1,-1);
        }
    }
}


//----------------------------------
void Singleton::DrawDepthCfg(){
    
    int width = DepthDialogWidth + 10;
    int height = DepthDialogHeigt + 45;
    int x = lake_DepthBar.getX() - 10;
    int y = lake_DepthBar.getY() - 45;
    pics.draw(-1, x, y, 0, false, width, height + 50, 0,
              PANELCOLOR, PANELCOLOR);
    pics.draw(8, x - 32, y, 0, false,
              32/64.0f, (height + 50)/128.0f, 0, PANELCOLOR, PANELCOLOR);
    pics.draw(8, x + width, y, 1, false,
              32/64.0f, (height + 50)/128.0f, 0, PANELCOLOR, PANELCOLOR);
    
    
    pics.draw(-1, x + 10, y + 45, 0, false, width - 20, DepthDialogHeigt/3.0f, 0,
              COLOR(1.0f, 1.0f, 1.0f), COLOR(0, 1.0f, 1.0f));
    pics.draw(-1, x + 10, y + 45 +(DepthDialogHeigt/3), 0, false, width - 20, DepthDialogHeigt/3.0f, 0,
              COLOR(0, 1.0f, 1.0f), COLOR(0,1.0f,0.0f));
    pics.draw(-1, x + 10, y + 45 +(DepthDialogHeigt/3*2), 0, false, width - 20, DepthDialogHeigt/3.0f, 0,
              COLOR(0, 1.0f, 0.0f), COLOR(0,0,1.0f));
    
    WriteShadedText(SCREEN_WIDTH/2 - 50, y + 10, pics, 0, lang[14], 1.8f, 1.8f, COLOR(1,1,1), COLOR(1,1,1),COLOR(0,0,0));
    
    pics.draw(-1, SCREEN_WIDTH/2, y + 45, 0, false, 1, DepthDialogHeigt, 0,
              COLOR(1.0f, 1.0f, 1.0f), COLOR(1.0f, 1.0f, 1.0f));
    Depth_OK.drawTextnPicture(pics, 7, 2, 0, lang[15]);
    
    
    int _height = lake_DepthBar.height;
    float height_percent = _height/100.0f;
    float depth_percent = 0.60f;
    int _deep = (MyRod.depth/depth_percent) * height_percent;
    
    pics.draw(4, SCREEN_WIDTH / 2 - 2,
              (y + 45 + DepthDialogHeigt) - _deep);
    
}
//---------------------------
void Singleton::DrawBucket(){
    pics.draw(-1, 
              BucketWindow.getX(),
              BucketWindow.getY(),
              0, false,
              BucketWindow.width,
              BucketWindow.height,
              0,
              PANELCOLOR, PANELCOLOR);
    pics.draw(8, BucketWindow.getX() - 32, BucketWindow.getY(), 0, false,
              32/64.0f, BucketWindow.height/128.0f, 0, PANELCOLOR, PANELCOLOR);
    pics.draw(8, BucketWindow.getX() + BucketWindow.width, BucketWindow.getY(), 1, false,
              32/64.0f, BucketWindow.height/128.0f, 0, PANELCOLOR, PANELCOLOR);
    
    
    showstatus(BucketWindow.getX(), BucketWindow.getY() + 10);
    
    Font.setcolor(0, 0, 0);
    BucketWindow_OK.drawTextnPicture(pics, 7, 2, 0, lang[15]);
    Font.setcolor(255, 255, 255);
}



//-----------------------
void Singleton::DrawLake(){
    char buf[255];
    LakeGfx.draw(3, 0, 60, 0, false,  1, 1, 0, COLOR(1,1,1, Alpha1), COLOR(0.5f, 0.5f, 0.5f, Alpha1));
    LakeGfx.draw(4, 0, 60, 0, false, 1, 1, 0, COLOR(1,1,1, Alpha2), COLOR(0.5f, 0.5f, 0.5f, Alpha2));
    LakeGfx.draw(1, 0, 0, 0, false);
    LakeGfx.draw(2, 0, 194);
    
    LakeGfx.drawBatch(666);
    if (uzmesta){
        
        glDisable(GL_TEXTURE_2D);
        DrawLine(LineStartX, LineStartY, thrownPoint.v[0], thrownPoint.v[1]);
        glEnable(GL_TEXTURE_2D);
        if (!pakirsta){
            float scale = pludey/(MAXFISHY*1.0f);
            scale = (scale < 0.9f)? scale + 0.1f : scale;
            pics.draw(16, pludex, thrownPoint.v[1] - 8*scale, MyRod.floatFrame, true, pludey/(MAXFISHY*1.0f), pludey/(MAXFISHY*1.0f));
        }
    }
    else{
        if (throwAnimationTic > 25){
            //LakeGfx.drawBatch(666);
            glDisable(GL_TEXTURE_2D);
            DrawLine(LineStartX, LineStartY, thrownAnimPoint.v[0], thrownAnimPoint.v[1]);
            glEnable(GL_TEXTURE_2D);
        }
    }
    
    
    LakeGfx.draw(5, ((uzmesta || (throwAnimationTic > 25))&&(thrownPoint.v[0] < LINESTARTXL))? LINESTARTXL - 16 : LINESTARTXL, MAXFISHY-8, ZvejasFrame);
    
    if (tooMuchFishMessageActive){
        pics.draw(15, LINESTARTXL, MAXFISHY - 60, 0, false, 1.5f, 1.0f);
        WriteText(LINESTARTXL + 70, MAXFISHY - 50, pics, 0, "I need to sell",0.8f, 0.8f, COLOR(0,0,0), COLOR(0,0,0));
        WriteText(LINESTARTXL + 70, MAXFISHY - 30, pics, 0, "my catch first", 0.8f, 0.8f, COLOR(0,0,0), COLOR(0,0,0));
    }
   
    DrawClouds();
    pics.drawBatch(666);
    
    if (LakeGfx.count() > 6)
        LakeGfx.draw(6, 0, 260 - 256);
    
    LakeGfx.draw(0, 0, 228);
    
    //frame3d(pics, 0, 0, 479, 231, false);
    
#ifdef _DEBUG_
    xbox(MINFISHX, MAXFISHY - MAXPOWER, MAXFISHX, MAXFISHY, TCOLOR(255, 0, 0));
#endif
    
    //depth meter
    sprintf(buf, "%d", MyRod.depth);
    
    NoMusic_Button.draw(pics, 2, 5 + NoMusic_Button.state);
      
    COLOR d = MapDepthColor(MyRod.depth);
    if (GylisCfg_Active)
        lake_depth.c = d;
    else 
        lake_depth.c = COLOR(lake_depth.c.c[0] * d.c[0], 
                         lake_depth.c.c[1] * d.c[1],
                         lake_depth.c.c[2] * d.c[2]);
    lake_depth.drawTextnPicture(pics,0, 2, 0, buf);
    
    if (!uzmesta)
        lake_throw.draw(pics, 2, 3);
    else
        lake_throw.draw(pics, 2, 4);
    
    pics.draw(2, lake_bait.getX(), lake_bait.getY(), 0, false, 1.0f, 1.0f, 0, 
              COLOR(0.33f*lake_bait.c.c[0], 0.33f*lake_bait.c.c[1], 0.33f*lake_bait.c.c[2]),
              COLOR(0.33f*lake_bait.c.c[0], 0.33f*lake_bait.c.c[1], 0.33f*lake_bait.c.c[2]));
    pics.draw(3, lake_bait.getX(), lake_bait.getY(), masalas, false, 1.0f, 1.0f, 0,
              lake_bait.c,lake_bait.c);
    lake_buckit.draw(pics, 2, 1);
    lake_exit.draw(pics, 2, 2);
    
    
    drawFishXY(335, 233);
    //drawFishYZ();
    DrawLRBar(lake_LRBar.getX(), lake_LRBar.getY());
    if (uzsimota || throwAnimation)
        DrawPowerBar(300, 30, COLOR(1.0f, 0, 0));
    if ((sys.useLineTension)&&(pakirsta))
        DrawTensionBar(300, 30);
    if (!uzmesta)
        DrawWindBar(300, 10);
    
   
    
    if ((!uzmesta)&&(uzkibo)&&((hand.rollin)||(hand.display)||(hand.rollout))){
        Font.setcolor(0,0,0);
        Font.display(15, 253, lang[19]);
        Font.setcolor(255, 255, 255);
        Font.display(14, 252, lang[19]);
        FishType * ft = fishData.getType(fishes[zuvID].kind);
        sprintf(buf, "%s %dg", ft->name, fishes[zuvID].mase);
        Font.setcolor(0,0,0);
        Font.display(15, 273, buf);
        Font.setcolor(255, 255, 255);
        Font.display(14, 272, buf);
        DrawHandnFish(hand.x, 20);
    }
    
    if (MasalasCfg_Active)
        DrawBaitCfg();
    if (GylisCfg_Active)
        DrawDepthCfg();
    if (Buckit_Active)
        DrawBucket();
    
    LakeGfx.drawBatch(666);
}
//------------------------------
void Singleton::HandAnimation(){
    if ((!uzmesta)&&(uzkibo)){
        if (hand.rollin){
            hand.tim++;
            if (hand.tim == 2){
                hand.x++;
                hand.tim = 0;
                if (hand.x > -1){
                    hand.rollin = false;
                    hand.display = true;
                }
            }
        }
        if (hand.display){
            hand.tim ++;
            if (hand.tim > 200){
                hand.tim = 0;
                hand.rollout = true;
                hand.display = false;
            }
        }
        if (hand.rollout){
            hand.x--;
            if (hand.x < -127){
                hand.rollout = false;
            }
        }
    }
    
}

//------------------
void Singleton::DepthConfigLogic(){
    if (!uzmesta){
                   
        if ((MouseX > lake_DepthBar.getX()) && (MouseX<lake_DepthBar.getX()+lake_DepthBar.width)
            &&(MouseY >= lake_DepthBar.getY()) && (MouseY <= lake_DepthBar.getY()+lake_DepthBar.height) 
            && ((touches.down.count())||(touches.move.count()))){
                
            int height = lake_DepthBar.height;
            float height_percent = height/100.0f;
            float depth_percent = 0.60f;
            MyRod.depth = ((lake_DepthBar.getY()+lake_DepthBar.height - MouseY)/height_percent) * depth_percent;
        }
        
        
        if (touches.up.count()){
            int mx = touches.up[0].v[0];
            int my = touches.up[0].v[1];
            
            if (Depth_OK.isPointerOnTop(mx, my))
                GylisCfg_Active = false;
        }
        
    }
    
}

//------------------------------
void Singleton::onBuckit(){
    
    if (touches.up.count()){
        int mx = touches.up[0].v[0];
        int my = touches.up[0].v[1];
        if (BucketWindow_OK.isPointerOnTop(mx, my)){
            Buckit_Active = false;
        }
    }
}
//------------------------------------
void Singleton::fish_atc(unsigned index){
    Fish * z = &fishes[index];
    if (fishx < z->x)
        z->x--;
    else
        if (fishx > z->x)
            z->x++;
    if (fishy < z->y)
        z->y--;
    else
        if (fishy > z->y)
            z->y++;
}
//------------------------------------------
bool Singleton::fishSmellsBait(unsigned fishindex, unsigned baitIndex){
    FishType * f = fishData.getType(fishes[fishindex].kind);
    
    unsigned fishlikeswhat = f->getFoodIndex(baitIndex);
    
    return CirclesColide(fishx, fishy, f->foods[fishlikeswhat].kvapas, fishes[fishindex].x, fishes[fishindex].y, 1.0f);
}
//---------------------------------------
void Singleton::MoveRandomly(Fish* z){
    
    if (!z){
      //  puts("ouch");
        return;
    }
    
    FishType * ft = fishData.getType(z->kind);
    
    z->dir = rand() % 6 + 1;
    //printf("dir: %u\n", z->dir);
    switch(z->dir){
        case 1: //S
            if (z->y < MAXPOWER)
                z->y++;
            break;
        case 2:{//N
            if (z->y > 0)
                z->y--;
        } break;
        case 3:{//E
            if (z->x < MAXFISHX - MINFISHX)
                z->x++;
        } break;
        case 4:{//W
            if (z->x > 0)
                z->x--;
        } break;
        case 5:{//Up
            if (z->depth < ft->mindepth)
                z->depth ++;
        }break;
        case 6:{ //down}
            if (z->depth > 1)
                z->depth--;
        }
    }
            
    
}
//___________________________________________
void Singleton::FishAI(){
    
    for (unsigned i = 0; i < fishes.count(); i++){
        Fish * z = &fishes[i];
        if (!(z->dead)){
            //puts("ouch");
            z->tim++;
            FishType * ft = fishData.getType(z->kind);
            
            if (z->tim > ft->response){
                z->tim = 0;
                bool sawbait = false;
                if ((uzmesta)&&(!pakirsta)){
                    
                    if (ft->isEatingThis(masalas) && (MyRod.depth == z->depth)){
                        if ((fishSmellsBait(i, masalas))&&
                        ((fishx < z->x)||(fishx > z->x)
                         ||(fishy < z->y)||(fishy > z->y))) {
                            
                            fish_atc(i);
                            sawbait = true;
                            
                        }
                    }
                }
                
                if ((!sawbait)||(pakirsta && (zuvID == i))){
                    MoveRandomly(z);
                   
                    if (pakirsta && (zuvID == i)){
                        fishx = fishes[i].x;
                        fishy = fishes[i].y;
                        thrownPoint.v[0] = fishx + MINFISHX;
                        thrownPoint.v[1] = fishy + (MAXFISHY - MAXPOWER);
                        throwDir = Vector3D(LineStartX * 1.0f, LineStartY * 1.0f , 0)
                                - Vector3D(thrownPoint.v[0] * 1.0f, thrownPoint.v[1] * 1.0f, 0.0f);
                        throwDir.normalize();
                    }
                }
            }
        }
        FishType * ft = fishData.getType(z->kind);
        if ((z->x == fishx) && (z->y == fishy) 
            && (MyRod.depth == z->depth) && (uzmesta) && (!uzkibo) && (!z->dead) &&
            (ft->isEatingThis(masalas))){
                //PludeFrame = 1;
                uzkibo = true;
                zuvID = i;
            }
    } //for
    
    if ((uzmesta)&&(uzkibo)&&(!pakirsta)){
        Ftim++;
        if (Ftim > 200){
            uzkibo=false;
            if (masalas == 3){
                me.getBaitByType(3)->count--;
                if (me.getBaitByType(3)->count <= 0){
                    int _index = me.getBaitIndexByType(3);
                    if (_index > -1){
                        me.removeBait((unsigned)_index);
                        Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                    }
                }
            }
            if (masalas == 4){
                me.getBaitByType(4)->count--;
                if (me.getBaitByType(4)->count <= 0){
                    int _index = me.getBaitIndexByType(4);
                    if (_index > -1){
                        me.removeBait((unsigned)_index);
                        Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                    }
                }
                    
            }
            masalas = 0;
            MyRod.floatFrame = 0;
            MyRod.floatFrameTics = 0;
            Ftim = 0;
            zuvID = 0;
        }
    }
}
//-------------------------------
void Singleton::PullOut(){
    
    if (uzmesta){
        if (thrownPoint.v[0] < 120) 
            ZvejasFrame = 5;
        else 
            ZvejasFrame = 2;
        
        uzmesta = false;
        
        wind = rand()%48 - 24;
        
        finishedPullOut = true;
        Ftim = 0;
        if (uzkibo){
            
            krepsis_def += fishes[zuvID].mase;
            
            MyRod.floatFrame = 0;
            MyRod.floatFrameTics = 0;
            
            me.addCatch(fishes[zuvID].kind, fishes[zuvID].mase);
            
            if (fishes[zuvID].kind == 2){
                if ((masalas > 2)&&(masalas < 5)){
                    me.getBaitByType(masalas)->count--;
                    if (me.getBaitByType(masalas)->count <= 0){
                        int _index = me.getBaitIndexByType(masalas);
                        if (_index > -1){
                            me.removeBait((unsigned)_index);
                            Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                        }
                    }
                }
            }
            hand.rollout = false;
            hand.rollin = true;
            if ((masalas != 3)&&(masalas != 4)) //LOL!
                masalas = 0;
            else
                if (fishes[zuvID].kind == 2)
                    masalas = 0;
            
            fishes[zuvID].dead = true;
        }
    }
}

//_______________________________
void Singleton::Throw(){
    if (!uzmesta){
        uzkibo = false;
        
        if ( UZMETSTARTX + lr + wind > MAXFISHX)
            thrownPoint.v[0] = MAXFISHX; 
        else
            if (UZMETSTARTX + lr + wind < MINFISHX)
                thrownPoint.v[0] = MINFISHX;
            else
                thrownPoint.v[0] = UZMETSTARTX + lr + wind;
        thrownPoint.v[1] = MAXFISHY - power;
        
        LineStartX = LINESTARTXR;
        if (thrownPoint.v[0] < LINESTARTXL){
            LineStartX = LINESTARTXL;
            ZvejasFrame = 5;
        } else
            if (thrownPoint.v[0] > LINESTARTXR)
                ZvejasFrame = 2;
            else
                ZvejasFrame = 2;
        
        
        if (thrownPoint.v[0] > MAXFISHX - 1)
            pludex = MAXFISHX - 1;
        else
            pludex = thrownPoint.v[0];
        pludey = thrownPoint.v[1] - 8;
        
        
       
        fishx = thrownPoint.v[0] - MINFISHX;
        fishy = thrownPoint.v[1] - (MAXFISHY - MAXPOWER);
        
        Vector3D start(LineStartX * 1.0f, LineStartY * 1.0f, 0.0f);
        throwDir = start - Vector3D(thrownPoint.v[0] * 1.0f, thrownPoint.v[1] * 1.0f, 0.0f);
        throwDir.normalize();
        //printf("%f %f\n", throwDir.v[0], throwDir.v[1]);
    }
}

//________________________________}
void Singleton::CheckPower(int max){
    
    if (uzsimota){
        if(!touches.up.count()){
            power++;
            if (power > max)
                power = 0;
            
        }
        else{
            uzsimota = false;
            throwAnimation = true;
           
        }
    }
}
//-----------------------------
void Singleton::CheckPullout(){
    
   
    float pullSpeed = 1.60f;
    if (uzkibo){
        pullSpeed = 1.60f - fishes[zuvID].mase * 0.001f;
    }
    thrownPoint = thrownPoint + 
                  Vector3D(throwDir.v[0] * pullSpeed, throwDir.v[1] * pullSpeed, 0);
    
    if ((uzkibo)&&(pakirsta)&&(sys.useLineTension)){
        LineTension += 2;
        if (LineTension > LineTensionMax){
            ss.playsound(4);
            LineTension = 0;
            pakirsta = false;
            if (masalas == 3){
                me.getBaitByType(3)->count--;
                if (me.getBaitByType(3)->count <= 0){
                    int _index = me.getBaitIndexByType(3);
                    if (_index > -1){
                        me.removeBait((unsigned)_index);
                        Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                    }
                }
            }
            if (masalas == 4){
                me.getBaitByType(4)->count--;
                if (me.getBaitByType(4)->count <= 0){
                    int _index = me.getBaitIndexByType(4);
                    if (_index > -1){
                        me.removeBait((unsigned)_index);
                        Bait_Area.setpos(Bait_Area.getX(), Bait_CfgWindow.getY() + 20);
                    }
                }
            }
            masalas = 0;
            zuvID = 0;
            uzkibo = false;
            MyRod.floatFrame = 0;
            MyRod.floatFrameTics = 0;
            
        }
    }
        
    
    
    if (((thrownPoint.v[0] < LineStartX + 3 ) && (thrownPoint.v[0] > LineStartX - 3))
        && ((thrownPoint.v[1] < LineStartY + 3) && (thrownPoint.v[1] > LineStartY - 3))){
        if (pakirsta)
            pakirsta = false;
        PullOut();
        LineTension = 0;
    }
    
    if (thrownPoint.v[0] > MAXFISHX - 1)
        pludex = MAXFISHX - 1;
    else
        pludex = thrownPoint.v[0];
    pludey = thrownPoint.v[1] - 8;
    
    fishx = thrownPoint.v[0] - MINFISHX;
    fishy = thrownPoint.v[1] - (MAXFISHY - MAXPOWER);
    
    if (pakirsta){
        fishes[zuvID].x = fishx;
        fishes[zuvID].y = fishy;
    }
}


//----------------------------
void Singleton::CheckLRButtons(){
    
    if ((touches.down.count())||(touches.move.count())){
        if (lake_LRBar.isPointerOnTop(MouseX, MouseY)){
            fingerOnLR = true;
            //int halfField = (MAXFISHX - MINFISHX) / 2;
            float bar_percent = (LR_WIDTH/100.0f);
            float field_percent = (MAXFISHX - MINFISHX)/100.0f;
            
            lr = (((MouseX - lake_LRBar.getX()) - (LR_WIDTH/2.0))/bar_percent)
            * field_percent;
            //printf("%d\n", lr);
        }
    }
    
    if ((!uzmesta)&&(!throwAnimation)){
        ZvejasFrame = (lr >= 0)? 0 : 3;
    }
}
//----------------------
void Singleton::exitLake(){
    if ((!uzmesta)&&(!uzsimota)){
        GameState = LAKESELECT;
        GenerateClouds(40);
        //Dude.x = 410;
        //TikslasX = Dude.x - 10;
        uzkibo = false;
        tooMuchFishMessageActive = false;
    }
    else{
        if ((!ss.isPlaying(0))){
            ss.playsound(0, false);
        }

    }

}

//-----------------------
void Singleton::LakeLogic(){
    
    if (MasalasCfg_Active){
        baitCfgLogic();
        return;
    }
    if (GylisCfg_Active){
        DepthConfigLogic();
        return;
    }
    if (Buckit_Active){
        onBuckit();
        return;
    }
    


    if ((OldKeys[6])&&(!Keys[6]))
        exitLake();
    
    
    if (!WaterSwitch){
        //Alpha1 += 0.01f;
        Alpha2 -= 0.008f;
        if (Alpha2 <= 0.0f){
            WaterSwitch = true;
        }
    }
    else{
        //Alpha1 -= 0.01f;
        Alpha2 += 0.008f;
        if (Alpha2 >= 1.0f){
            WaterSwitch = false;
        }
        
    }
    
    if (throwAnimation){
        throwAnimationTic++;
        if (throwAnimationTic == 12){
            ZvejasFrame++;
           // printf("zf: %d\n", ZvejasFrame);
        }
        if (throwAnimationTic == 25){
            Throw();
            thrownAnimPoint = Vector3D(LineStartX, LineStartY,0);
        }
        if (throwAnimationTic > 25){
            
            float step = 2.0f;
            thrownAnimPoint = thrownAnimPoint - Vector3D(throwDir.v[0]*step, throwDir.v[1]*step, 0);
            if (throwAnimationTic > 37){
                throwAnimation = false;
                throwAnimationTic = 0;
                ss.playsound(3, false);
                uzmesta = true;
                MyRod.floatFrame = 0;
                MyRod.floatFrameTics = 0;
                
            }
        }
    }
    
    
    MoveClouds(30);
    
    HandAnimation();
    
    
    FishAI();
    
    if ((uzkibo)&&(!pakirsta)){
        MyRod.floatFrameTics++;
        if (MyRod.floatFrameTics > 5){
            MyRod.floatFrameTics = 0;
            MyRod.floatFrame++;
            if (MyRod.floatFrame >= 4)
                MyRod.floatFrame = 1;
        }
    }
    
    CheckPower(MAXPOWER);
    if (!uzmesta)
        CheckLRButtons();
    
    
    
    if ((uzmesta)&&(pakirsta)&&(sys.useLineTension)){
        if (LineTension > 0)
            LineTension--;
    }
    
    if (touches.down.count()){
        int mx = touches.down[0].v[0];
        int my = touches.down[0].v[1];
        
        COLOR dark = COLOR(0.5f, 0.5f, 0.5f);
        if (lake_throw.isPointerOnTop(mx, my))
            lake_throw.c = dark;
        if (lake_bait.isPointerOnTop(mx, my))
            lake_bait.c = dark;
        if (lake_depth.isPointerOnTop(mx, my))
            lake_depth.c = dark;
        if (lake_buckit.isPointerOnTop(mx, my))
            lake_buckit.c = dark;
    }
    
    if (touches.up.count()){
        
        int mx = touches.up[0].v[0];
        int my = touches.up[0].v[1];
        
        if (lake_bait.isPointerOnTop(mx, my)){
            lake_bait.c = COLOR(1,1,1);
            if (!fingerOnLR){
                if (!uzmesta){
                    masalasold = masalas;
                    MasalasCfg_Active = true;
                    oldMoveTouch = Vector3D(-1,-1,-1);
                }
                else{
                    if ((!ss.isPlaying(0))){
                        ss.playsound(0, false);
                    }
                }
            }
        }
    
        if (lake_depth.isPointerOnTop(mx, my)){
            lake_depth.c = COLOR(1,1,1);
            if (!fingerOnLR){
                if ((!uzmesta)&&(!uzsimota)){
                    GylisCfg_Active = true;
                }
                else{
                    if (!ss.isPlaying(0)){
                        ss.playsound(0, false);
                    }
            
                }
            }
        }
        
        if (lake_LRBar.isPointerOnTop(mx, my)){
            fingerOnLR = false;
        }
    
        if (lake_buckit.isPointerOnTop(mx, my)){
            lake_buckit.c = COLOR(1,1,1);
            Buckit_Active = true;
        }
        
        if (lake_throw.isPointerOnTop(mx, my)){
            lake_throw.c = COLOR(1,1,1);
            
            if (finishedPullOut)
                finishedPullOut = false;
        }
    
        if (lake_exit.isPointerOnTop(mx, my)){  
            exitLake();
        }
    }
    
    
    if (!touches.allfingersup){
        
        if ((NoMusic_Button.isPointerOnTop(MouseX, MouseY))&&(touches.oldDown.count())){
            if (!NoMusic_Button.state){
                music.stop();
                NoMusic_Button.state = 1;
            }
            else{
                music.playback();
                NoMusic_Button.state = 0;
            }
        }
        
        if (lake_throw.isPointerOnTop(MouseX, MouseY)){
            
            if ((!fingerOnLR)&&(!(hand.rollin && (hand.x < -30) && (uzkibo)))){
            
                if ((!uzmesta)&&(!uzsimota)&&(!throwAnimation)&&(!finishedPullOut)) {
                    if (krepsis_def > krepsis_max){
                        tooMuchFishMessageActive = true;
                    }
                    else{
                        uzsimota = true;
                        power = 0;
                        hand.reset();
                    }
                }
                else{
                    if (uzmesta){
                        if ((uzmesta)&&(uzkibo)&&(!pakirsta))
                            pakirsta = true;
                        if (!ss.isPlaying(1))
                            ss.playsound(1, false);
                        CheckPullout();
                    }   
                }
            }
        
        }
        
    }
    else{
        COLOR light = COLOR(1.0f, 1.0f, 1.0f);
        lake_throw.c = light;
        lake_bait.c = light;
        lake_depth.c = light;
        lake_buckit.c = light;
        
        fingerOnLR = false;
        
        
        clickCount = 0;
        if (finishedPullOut)
            finishedPullOut = false;
    }
}
//-------------------------------------------
void Singleton::DrawShop(){
    
    char buf[255];
    pics.draw(9, 0, 0, 0, false, 480.0f/512.0f, 320.0f/256.0f);
   
   
    for (unsigned i = 1; i < baitData.count(); i++){
        
        TBait * b = baitData.getBait(i);
        pics.draw(3, Shop_GoodsArea.getX(), Shop_GoodsArea.getY() + (48*(i-1)), i, false);
        
        if (b->countInShop > 1)
            sprintf(buf, "%13s  %3d%2s  %2d$", 
                    b->name, b->countInShop,
                    b->units, b->price);
        else
            sprintf(buf, "%13s  %5s  %2d$", b->name, " ", b->price);
        Font.display(Shop_GoodsArea.getX() + 60, Shop_GoodsArea.getY()+(48*(i-1))+10, buf);
    }
    
    pics.draw(-1, 470, 50, 0, 0, 5, 200, 0, COLOR(0.5f, 0.5f, 0.5f, 0.6f), COLOR(0.1f, 0.1f, 0.1f, 0.6f));
    pics.draw(-1, 470, 50+(80-Shop_GoodsArea.getY())/1.44, 0, 0, 5, 150, 0, 
              COLOR(0.7f, 0.7f, 0.7f, 0.6f), COLOR(0.7f, 0.7f, 0.7f, 0.6f));
    
    COLOR red = COLOR(1.0f, 0, 0);
    if (preke)
        xbox(pics,
             Shop_GoodsArea.getX()-5,
             Shop_GoodsArea.getY()+48*(preke-1),
             Shop_GoodsArea.getX() + 290,
             Shop_GoodsArea.getY()+48*preke, red); 
    
    pics.draw(5, 130, 256, 0);
    Font.setcolor(1,1,1);
    shop_buy.drawTextnPicture(pics, 7, 2, 0, lang[10]);
    shop_exit.drawTextnPicture(pics,7, 2, 0, lang[11]);
    Font.setcolor(0.2, 0.2, 0.2);
    sprintf(buf, "%s : %.2f $", lang[5], me.money());
    Font.display(160, 290, buf);
    
    
    
    
    pics.draw(15, 40, -10, 0, false, 400/137.0f, 100/64.0f);
    Font.setcolor(0,0,0);
    Font.display(160, 20, shopKeeperSays);
    Font.setcolor(255,255,255);
    
}
//--------------------------------
void Singleton::ShopLogic(){
    
    if (touches.up.count()){
        oldMoveTouch = Vector3D(-1,-1,0);
        
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];
        
        Shop_GoodsArea.height = (int)48*(baitData.count()-1);
        if ((Shop_GoodsArea.isPointerOnTop(mx, my))&&
             (MouseY < 256)){
        
            preke = (MouseY - Shop_GoodsArea.getY())/48 + 1; 
           // printf("preke: %d\n", preke);
        }
        
        if (shop_buy.isPointerOnTop(mx, my)){
            if (preke){
                TBait* b = baitData.getBait(preke);
                
                if (me.money() >= b->price){
                    
                    MBait* mb = 0;
                    //printf("-preke: %d\n", preke);
                    mb = me.getBaitByType(preke);
                    
                    if (mb){
                        
                        if (b->maxCount <= mb->count){
                            strcpy(shopKeeperSays,"You already have enough.");
                            preke = 0;
                            return;
                        }
                        mb->count += b->countInShop;
                    }
                    else{
                        me.addBait(preke, b->countInShop);
                    }
                    me.setMoney(me.money() - b->price);
                    ss.playsound(2, false);
                }
                else{
                    preke = 0;
                    ss.playsound(0, false);
                    
                    strcpy(shopKeeperSays,"Short on cash ?");
                }
            }
        }
        
        if (shop_exit.isPointerOnTop(mx, my)){
            GameState = TOWN;
            strcpy(shopKeeperSays,"What would you like to buy ?");
        }
        
        return;
    }
    
    if (touches.move.count()){
        Vector3D m = touches.move[0];
        
        if (oldMoveTouch.v[0] > -1){
            Vector3D diff = m - oldMoveTouch;
        
            int minY = 256-48*(baitData.count()-1);
            if ((Shop_GoodsArea.getY()+ diff.v[1] < 80)&&(Shop_GoodsArea.getY()+ diff.v[1] > minY))
                Shop_GoodsArea.setpos(Shop_GoodsArea.getX(), Shop_GoodsArea.getY() + diff.v[1]);
        }
        
        oldMoveTouch = touches.move[0];
    }
    
}
//-------------------------------
void Singleton::showfish(int x, int y, int c, int ind){
    int fx = x; int fy = y;
    for (int i = 0; i < c; i++){
        pics.draw(11+ind, fx, fy, 0, true, 0.4f, 0.4f, 45.0f - rand() % 10 );
        fx = fx + 15;
        //fy = fy + 2;
    }
}
//-------------------------------
void Singleton::DrawRestaurant(){
    pics.draw(17, 0, 0, 0, false, 480.0f/512.0f, 320.0f/256.0f);
   
    pics.draw(-1, 470, 50, 0, 0, 5, 200, 0, COLOR(0.5f, 0.5f, 0.5f, 0.6f), COLOR(0.1f, 0.1f, 0.1f, 0.6f));
    float barheight = (me.differentFishCount()<= 3)?200.0f : (200.0f/100.0f) * (3.8f/(me.differentFishCount()/100.0f));
    float barY = 50+(100-Rest_FishArea.getY())/(((me.differentFishCount())*50.0f)/200.0f);
    pics.draw(-1, 470, barY, 0, 0, 5, barheight, 0, 
              COLOR(0.7f, 0.7f, 0.7f, 0.6f), COLOR(0.7f, 0.7f, 0.7f, 0.6f));
   
    Font.setcolor(1, 1, 1);
    double total = 0;
    char buf[255];
    if (me.catchCount()) {
    
        int index = 0;
        for (unsigned i = 0; i < fishData.count(); i++){
            unsigned fishindex = (fishData.count() - 1) - i;
            int w = me.catchWeightByKind(fishindex);
            if (w){
                FishType * ft = fishData.getType(fishindex);
                showfish(Rest_FishArea.getX(),  Rest_FishArea.getY() + 50 * index,
                         me.catchCountByKind(fishindex), ft->picture);
                sprintf(buf, "%dx%.3f = %.2f$", w, (ft->price/1000.0f),
                        (w * (ft->price / 1000.0f)));
                Font.display(Rest_FishArea.getX() + 125, Rest_FishArea.getY()+5+ 50 * index, buf);
                
                total +=  w * (ft->price / 1000.0f);
                index++;
            }
        }
        
        pics.draw(15, 40, -10, 0, false, 420/137.0f, 100/64.0f);
        Font.setcolor(0, 0, 0);
        Font.display(160, 10, lang[13]);
    }
    else{
        pics.draw(15, 40, -10, 0, false, 420/137.0f, 100/64.0f);
        Font.setcolor(0, 0, 0);
        Font.display(157, 20, lang[12]);
    }
    
    pics.draw(5, 130, 256, 0);
    Font.setcolor(0, 0, 0);
    
    Restaurant_exit.drawTextnPicture(pics, 7, 2, 0, lang[11]);
    
    
    
    if (total > 0.00){
        Restaurant_sell.drawTextnPicture(pics, 7, 2, 0, "Sell");
        sprintf(buf, "Total : %.2f $", total);
        Font.setcolor(0.2f, 0.2f, 0.2f);
        Font.display(320, 290, buf);
    }
}

//-------------------------------
void Singleton::punktas(){
    
    if (touches.up.count()){
        
        oldMoveTouch = Vector3D(-1,-1,0);

        
        int mx = touches.up[0].v[0];
        int my = touches.up[1].v[1];
        
        //printf("dif fish: %d\n",me.differentFishCount());
        Rest_FishArea.height = (int)50*(me.differentFishCount()-1);
        
        if (Restaurant_sell.isPointerOnTop(mx, my)){
            if (me.catchCount() > 0){
                for (unsigned i = 0; i < fishData.count(); i++){
                    int w = me.catchWeightByKind(i);
                    FishType * ft = fishData.getType(i);
                    me.setMoney(me.money() + (w * (ft->price / 1000.00)));
                }
                me.destroyCatch();
                krepsis_def = 0;
                ss.playsound(2, false);
                Rest_FishArea.setpos(Rest_FishArea.getX(), 100);
            }
            //GameState = TOWN;
        }
        if (Restaurant_exit.isPointerOnTop(mx, my)){
            GameState = TOWN;
        }
    }
    
    if (touches.move.count()){
        Vector3D m = touches.move[0];
        
        if (oldMoveTouch.v[0] > -1){
            Vector3D diff = m - oldMoveTouch;
            
            int minY = 240-50*(me.differentFishCount()-1);
            if ((Rest_FishArea.getY()+ diff.v[1] < 100)&&(Rest_FishArea.getY()+ diff.v[1] > minY))
                Rest_FishArea.setpos(Rest_FishArea.getX(), Rest_FishArea.getY() + diff.v[1]);
        }
        
        oldMoveTouch = touches.move[0];
    }
    
}
//-------------------------------
void Singleton::DrawCredits(){
    draw_intro();
    int x = SCREEN_WIDTH/2 - 180;
    int y = 25;
    int width = 360;
    int height = 280;
    COLOR c1 = COLOR(0.5f,0.0f,0.8f, 0.9f);
    COLOR c2 = COLOR(0.2f,0.0f,0.3f, 0.7f);
    pics.draw(-1, x, y, 0, false, width, height, 0, 
              c1, c2);
    
    pics.draw(8, x - 32, y, 0, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    pics.draw(8, x + width, y, 1, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    
    pics.draw(18, SCREEN_WIDTH/2, 32, 1, true);
    
    Font.setcolor(0,0,0);
    
    Font.display(SCREEN_WIDTH/2 - 73, 61, "Code & Graphics:");
    Font.display(SCREEN_WIDTH/2 - 28, 81, "jrs0ul");
    //WriteShadedText(SCREEN_WIDTH/2 - 82, 101, pics, 0, "http://jrs0ul.com/");
    Font.display(SCREEN_WIDTH/2 - 77, 171, "Awesome music by:");
    Font.display(SCREEN_WIDTH/2 - 122, 201, "Fonogeri from album 'Fable'");
    Font.display(SCREEN_WIDTH/2 - 172, 221, "http://www.jamendo.com/en/album/53232");
    Font.setcolor(1,1,1);
    
    Font.display(SCREEN_WIDTH/2 - 72, 60, "Code & Graphics:");
    Font.display(SCREEN_WIDTH/2 - 27, 80, "jrs0ul");
    Font.display(SCREEN_WIDTH/2 - 76, 170, "Awesome music by:");
    Font.display(SCREEN_WIDTH/2 - 121, 200, "Fonogeri from album 'Fable'");
    Font.display(SCREEN_WIDTH/2 - 171, 220, "http://www.jamendo.com/en/album/53232");
}
//------------------------------
void Singleton::Credits(){
    
    
    if (((touches.oldDown.count())&&(!touches.down.count()))){
        
        if (jrs0ul_button.isPointerOnTop(MouseX,MouseY))
            launchjrs0ul = true;
        else
            GameState = INTRO;
    }
}
//-----------------------------
void Singleton::DrawHelp(){
    draw_intro();
    int x = SCREEN_WIDTH/2 - 200;
    int y = 5;
    int width = 400;
    int height = 310;
    COLOR c1 = COLOR(0.5f,0.0f,0.8f, 0.9f);
    COLOR c2 = COLOR(0.2f,0.0f,0.3f, 0.7f);
    pics.draw(-1, x, y, 0, false, width, height, 0, 
              c1, c2);
    
    pics.draw(8, x - 32, y, 0, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    pics.draw(8, x + width, y, 1, false,
              32/64.0f, height/128.0f, 0, c1, c2);
    pics.draw(18, SCREEN_WIDTH/2, 32, 2, true);
    
    char pages[4][11][255] = {{"Game consists of three modes: the town, the lake map,",
                              "and the fishing spot.",
                              " ",
                              "                       -Town-",
                              "Here you can find all shops. The bait shop is in the",
                              "left middle of the screen.",
                              "On the middle right you can sell your catch.",
                              "On far right is the lake. It's your main direction",
                              "to start fishing.",
                              "On the far left is your home. If you go there,",
                              "you'll be returned to main menu."
                              },
                            {" ",
                             "                      -Lake Map-",
                             "You should pick a place for a fishing or return back ",
                             "to town.",
                             "Every spot is somewhat unique."},
                            {},
                            {" ",
                             "                       -Fish- ",
                             "Roaches and zanders, perches and ruffes, and also",
                             "breams like worms.",
                             "Pikes, perches, and zanders will grab spinner and",
                             "twister lures.",
                             "Roaches, breams, and ruffes love paste and maggots.",
                            }};
    
    switch (activeHelpPage){
        case 0:{
            for (unsigned i = 0; i < 11; i++)
                WriteShadedText(SCREEN_WIDTH/2 - 220, 50+i*20, pics, 0, pages[0][i], 0.95f);
            
        } break;
        case 1:{
            for (unsigned i = 0; i < 11; i++)
                WriteShadedText(SCREEN_WIDTH/2 - 220, 50+i*20, pics, 0, pages[1][i], 0.95f);
        }break;
        
        case 2:{
            
            WriteShadedText(SCREEN_WIDTH/2 - 41, 50, pics, 0, "-Lake-");
            WriteShadedText(SCREEN_WIDTH/2 - 220, 80, pics, 0, "It's the place where you catch fish.", 0.95f);
            WriteShadedText(SCREEN_WIDTH/2 - 220, 100, pics, 0, "There are some buttons on the screen:", 0.95f);
            pics.draw(2, 20, 115, 3, false, 0.5f, 0.5f);
            WriteShadedText(42, 125, pics, 0, "/", 0.9f);
            pics.draw(2, 50, 115, 4, false, 0.5f, 0.5f);
            WriteShadedText(90, 120, pics, 0, "- Throw/Pull", 0.9f);
            pics.draw(2, 220, 115, 1, false, 0.5f, 0.5f);
            WriteShadedText(245, 120, pics, 0, "- Shows your catch", 0.95f);
            pics.draw(2, 20, 140, 2, false, 0.5f, 0.5f);
            WriteShadedText(40, 145, pics, 0, " - Goes to town", 0.95f);
    
            
            pics.draw(2, 20, 165, 0, false, 0.5f, 0.5f, 0 , MapDepthColor(20), MapDepthColor(20));
            Font.display(27, 172, "20", 0.5f, 4.0f);
            pics.draw(2, 125, 165, 0, false, 0.4f, 0.4f, 0 , MapDepthColor(55), MapDepthColor(55));
            pics.draw(2, 205, 165, 0, false, 0.4f, 0.4f, 0 , MapDepthColor(25), MapDepthColor(35));
            pics.draw(2, 280, 165, 0, false, 0.4f, 0.4f, 0 , MapDepthColor(20), MapDepthColor(20));
            WriteShadedText(40, 166, pics, 0, " - Depth (", 0.95f);
            WriteShadedText(130, 166, pics, 0, "   Surface    Middle    Bottom )", 0.8f);
            
            pics.draw(2, 20, 190, 0, false, 0.5f, 0.5f, 0, COLOR(0.5,0.5,0.5), COLOR(0.5,0.5,0.5));
            pics.draw(3, 20, 190, 0, false, 0.5f, 0.5f, 0);
            WriteShadedText(40, 195, pics, 0, " - Bait", 0.95f);
    
            WriteShadedText(SCREEN_WIDTH/2 - 215, 220, pics, 0, "If the bait button is exactly like this", 0.95f); 
            WriteShadedText(SCREEN_WIDTH/2 - 215, 240, pics, 0, "you'll need to set your bait. To do so, tap this", 0.95f); 
            WriteShadedText(SCREEN_WIDTH/2 - 215, 260, pics, 0, "button and select a bait you want", 0.95f);
           
    
        }break;
            
        case 3:{
            for (unsigned i = 0; i < 11; i++)
                WriteShadedText(SCREEN_WIDTH/2 - 220, 50+i*20, pics, 0, pages[3][i], 0.95f);
           
        }
    }
    char buf[255];
    sprintf(buf, "%u/%u", activeHelpPage+1, maxHelpPage+1);
    WriteShadedText(220, 290, pics, 0, buf);
    
    if (activeHelpPage == maxHelpPage)
        nextHelpPage_button.c = COLOR(0.5f, 0.5f, 0.5f);
    else
        nextHelpPage_button.c = COLOR(1.0f, 1.0f, 1.0f);
    nextHelpPage_button.draw(pics, 2, 9);
    
    if (activeHelpPage == 0)
        prevHelpPage_button.c = COLOR(0.5f, 0.5f, 0.5f);
    else
        prevHelpPage_button.c = COLOR(1.0f, 1.0f, 1.0f);
    prevHelpPage_button.draw(pics, 2, 8);
    
    helpBack_button.drawTextnPicture(pics, 7, 2, 0, "back");
    
    
}
//--------------------------
void Singleton::Help(){
    
    if (touches.up.count()){
        
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];
        
        if (helpBack_button.isPointerOnTop(mx, my)){
            GameState = INTRO;
        }
        
        if (prevHelpPage_button.isPointerOnTop(mx, my)){
            if (activeHelpPage > 0)
                activeHelpPage--;
        }
        if (nextHelpPage_button.isPointerOnTop(mx, my)){
            if (activeHelpPage < maxHelpPage)
                activeHelpPage++;
        }
        
    }
}
//------------------------
void Singleton::drawFishingSpotSelection(){
    pics.draw(lakes[lakeIndex].bgIndex, 0, 0, 0, false, 480.0f/512.0f, 320.0f/256.0f);
    
    for (unsigned i = 0; i <  lakes[lakeIndex].spots.count(); i++){
        LakePoint * p = &lakes[lakeIndex].spots[i];
        if (!p->locked)
            pics.draw(19, p->point.v[0] + ((p->type)? fishingSpotsAnim : 0),
                          p->point.v[1] + ((!p->type)?fishingSpotsAnim:0), 
                          p->type, true);
    }
    
       
    if (loadingLakePics){
        int width = 150;
        int height = 50;
        int x = 240-width/2;
        int y = 160-height/2;
        COLOR c1 = COLOR(0.5f,0.5f,0.5f, 0.9f);
        COLOR c2 = COLOR(0.5f,0.5f,0.5f, 0.7f);
        pics.draw(-1, x, y, 0, false, width, height, 0, 
                  c1, c2);
        
        pics.draw(8, x - 32, y, 0, false,
                  32/64.0f, height/128.0f, 0, c1, c2);
        pics.draw(8, x + width, y, 1, false,
                  32/64.0f, height/128.0f, 0, c1, c2);
        char buf[50];
        strcpy(buf,"Loading   ");
        switch (loadingAnim/6) {
            case 1: buf[7]='.';buf[8]=' ';buf[9]=' '; break;
            case 2: buf[7]='.';buf[8]='.';buf[9]=' '; break;
            case 3: buf[7]='.';buf[8]='.';buf[9]='.'; break;
            default:buf[7]=' ';buf[8]=' ';buf[9]=' '; break;
                break;
        }
        WriteShadedText(x+30, y+20, pics, 0, buf);
    }
    
}
//-------------------------
void Singleton::LoadLakeImages(){
    unsigned short bits;
    for (unsigned i = 0; i < 6; i++){
        if (!LakePicsData[i].loadTga(LakeGfxList[i], bits))
            printf("can't load %s\n", LakeGfxList[i]);
    }
    if (strlen(LakeGfxList[6]) > 1){
        if (!LakePicsData[6].loadTga(LakeGfxList[6], bits))
            printf("can't load %s\n", LakeGfxList[6]);

    }
    
    
    loadingLakePics = false;
    lakePicsLoaded = true;

}

//-----------------------
void Singleton::enterLake(){
    
    LakeGfx.destroy();
    
    Thread t;
    t.create(&ThreadFunc, this);
    loadingLakePics = true;
    lakePicsLoaded = false;
    wind = rand()%48 - 24;
   
}



//------------------------
void Singleton::fishingSpotSelectionLogic(){
    
    if (loadingLakePics){
        loadingAnim++;
        if (loadingAnim > 21)
            loadingAnim = 0;
        return;
    }

    if ((OldKeys[6])&&(!Keys[6])){
        LakeGfx.destroy();
        GameState = TOWN;
    }
    
    if (lakePicsLoaded){
        lakePicsLoaded = false;
        LakeGfx.makeTexture(LakePicsData[0], LakeGfxList[0], 0, 480, 128,  1);
        LakeGfx.makeTexture(LakePicsData[1], LakeGfxList[1], 1, 480, 128,  1);
        LakeGfx.makeTexture(LakePicsData[2], LakeGfxList[2], 2, 480, 64, 1);
        LakeGfx.makeTexture(LakePicsData[3], LakeGfxList[3], 3, 480, 200, 0);
        LakeGfx.makeTexture(LakePicsData[4], LakeGfxList[4], 4, 480, 200, 0);
        LakeGfx.makeTexture(LakePicsData[5], LakeGfxList[5], 5, 80, 84, 0);
        if (strlen(LakeGfxList[6]) > 1)
            LakeGfx.makeTexture(LakePicsData[6], LakeGfxList[6], 6, 480, 256, 0);
        for (unsigned i = 0; i < 6; i++)
            LakePicsData[i].destroy();
        GameState = LAKE;
    }
    
    if (fishingSpotAnimationDir){
        fishingSpotsAnim++;
        if (fishingSpotsAnim > 10)
            fishingSpotAnimationDir = false;
    }
    else{
        fishingSpotsAnim--;
        if (fishingSpotsAnim < -10)
            fishingSpotAnimationDir = true;
    }
    
    if (touches.up.count()){
        
        int mx = touches.up[0].v[0];
        int my = touches.up[0].v[1];
        

        for (unsigned i = 0; i < lakes[lakeIndex].spots.count(); i++){

            LakePoint * p = &lakes[lakeIndex].spots[i];
        
            if (CirclesColide(p->point.v[0],
                              p->point.v[1],
                              32, mx, my, 2)){

                if (!p->locked){
                    if (p->type){
                        LakeGfx.destroy();
                        GameState = TOWN;
                    }
                    else{
                        if (strlen(lakes[lakeIndex].spots[i].lakepics[1]) > 1)
                            strcpy(LakeGfxList[1], lakes[lakeIndex].spots[i].lakepics[1]);
                        strcpy(LakeGfxList[6], lakes[lakeIndex].spots[i].lakepics[6]);
                        //int adress = (int)p->func;
                        if (p->func == 0){
                            enterLake();
                        }
                        else 
                            (*p->func)(this);
                        if (lakes[lakeIndex].spots.count())
                            Randomfish(lakes[lakeIndex].spots[i].fes);
                        return;
                    }
                }
            }
        }
    }
}
//-------------------
void Singleton::enterBoat(){
    GameState = BOAT;
    if (islandEnabled){
        islandEnabled = false;
        for (unsigned i = 0; i < lakes[0].spots.count(); i++)
            lakes[0].spots[i].locked = false;
        lakes[0].spots[6].locked = true;
    }

}
//--------------------
void Singleton::drawCat(){

    pics.draw(36, 0, 0, 0, false, 480.0f/512.0f, 320.0f/256.0f);
    pics.draw(5, 130, 256, 0);
    pics.draw(30, 170, -15, 0, false, 340/128.0f, 128/64.0f);
    if (!me.catchCountByKind(6)){
        if (!gotCatFishes){
            WriteText(240, 15, pics, 0, "OH HAI! Could you catch", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(240, 35, pics, 0, "the Great Catfish for me?", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(250, 55, pics, 0, "It can be caught only", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(280, 75, pics, 0, "from teh island.", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
        }
        else{

            WriteText(250, 15, pics, 0, "Here's your reward.", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(240, 35, pics, 0, "But there's should be ", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(250, 55, pics, 0, "MOAR catfishes!", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
            WriteText(280, 75, pics, 0, "Bring 'em here!", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
        }
    }
    else{
        WriteText(220, 35, pics, 0, "Ooh Catfishes! Gimme gimme!", 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
        catGive_button.drawTextnPicture(pics, 7, 2, 0, "Give");
    }
    catExit_button.drawTextnPicture(pics, 7, 2, 0, "Exit");
}
//--------------------
void Singleton::catLogic(){

    if (touches.up.count()){
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];


        if (catExit_button.isPointerOnTop(mx, my)){
            GameState = LAKESELECT;
        }

        if (me.catchCountByKind(6)){
            if (catGive_button.isPointerOnTop(mx, my)){
                int w = me.catchWeightByKind(6);
                FishType * ft = fishData.getType(6);
                me.setMoney(me.money() + (w * ((ft->price*3) / 1000.00)));
                me.removeCatchByType(6);
                krepsis_def -= w;
                ss.playsound(2, false);
                gotCatFishes = true;
            }

        }
    }
}


//-------------------
void Singleton::drawBus(){

    COLOR red = COLOR(1.0f, 0, 0);

    pics.draw(33, 0, 0);
    pics.draw(30, 140, -15, 0, false, 350/128.0f, 128/64.0f);
    WriteText(200, 35, pics, 0,
              busDriver[busDriverSpeachIndex],
              1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));

    char buf[255];
    int atomPrice = (SceneIndex == 2)? 0 : 10;
    sprintf(buf, "%13s  %5s  %2d$", "Atom Lake", " ", atomPrice);
    WriteText(BusTicketArea.getX() + 5, BusTicketArea.getY() + 5, pics, 0, buf);
    if (SceneIndex != 1 ){
        sprintf(buf, "%13s  %5s  %2d$", "Hometown", " ", 10);
        WriteText(BusTicketArea.getX() + 5, BusTicketArea.getY()+TicketItem + 5, pics, 0, buf);
    }


    if (ticketIndex)
        xbox(pics, BusTicketArea.getX(), BusTicketArea.getY() + (ticketIndex - 1) * TicketItem, 
            BusTicketArea.getX() + BusTicketArea.width,
            BusTicketArea.getY() + TicketItem*ticketIndex, red);
    
    pics.draw(5, 130, 256, 0);
    busExit_button.drawTextnPicture(pics, 7, 2, 0, "Exit");
    busGo_button.drawTextnPicture(pics, 7, 2, 0, "Go");
    Font.setcolor(0.2, 0.2, 0.2);
    sprintf(buf, "%s:%.2f$", lang[5], me.money());
    Font.display(145, 290, buf);
}

//-------------------
void Singleton::busLogic(){

    if (touches.up.count()){
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];

        if (busExit_button.isPointerOnTop(mx, my)){
            GameState = TOWN;
            if (SceneIndex == 2){
                Dude.x = 170;
                Dude.y = 150;
            }
            else{
                Dude.x = 360;
                Dude.y = 70;
            }
        }


        if (BusTicketArea.isPointerOnTop(mx, my)){
            if (busDriverSpeachIndex > 0)
                busDriverSpeachIndex = 0;
            
            if (SceneIndex != 1){
                ticketIndex = (my - BusTicketArea.getY())/TicketItem + 1;
                if (ticketIndex > 2)
                    ticketIndex = 2;
            }
            else
                ticketIndex = 1;

        }

        if (busGo_button.isPointerOnTop(mx, my)){

            if (ticketIndex){

                if ((ticketIndex == 1) && (SceneIndex == 2))
                    return;
                if (me.money() < 10){
                    busDriverSpeachIndex = 1;
                    return;
                }
                else{
                    me.setMoney(me.money() - 10);
                }

                GameState = TOWN;
                ss.playsound(2, false);

                switch(ticketIndex){
                    case 1: {SceneIndex = 2; lakeIndex = 1;}break;
                    case 2: {SceneIndex = 1; lakeIndex = 0;} break;
                }
                ticketIndex = 0;
            }

        }
    }


}


//--------------------
void Singleton::drawBoat(){
    char buf[100];
    pics.draw(29, 0, 0, 0, false, 480.0f/512.0f, 320.0f/256.0f);
    pics.draw(30, 130, -15, 0, false, 350/128.0f, 128/64.0f);
    WriteText(200, 20, pics, 0, boatMan[0], 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
    WriteText(200, 40, pics, 0, boatMan[1], 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
    if (strlen(boatMan[2])){
        
        sprintf(buf, boatMan[2], ferryPrice);
        WriteText(200, 60, pics, 0, buf, 1.0f, 1.0f, COLOR(0,0,0), COLOR(0,0,0));
    }
    pics.draw(5, 130, 256, 0);
    boatYes_button.drawTextnPicture(pics, 7, 2, 0, "Yes");
    boatNo_button.drawTextnPicture(pics, 7, 2, 0, "No");
    boatExit_button.drawTextnPicture(pics, 7, 2, 0, "Exit");
    
    Font.setcolor(0.2, 0.2, 0.2);
    sprintf(buf, "%s:%.2f$", lang[5], me.money());
    Font.display(145, 290, buf);
}
//--------------------
void Singleton::enterCat(){
    GameState = CAT;
    gotCatFishes = false;
}

//--------------------
void Singleton::boatLogic(){
    
    if (touches.up.count()){
        float mx = touches.up[0].v[0];
        float my = touches.up[0].v[1];
        
        if (boatYes_button.isPointerOnTop(mx, my)){
            
            if (me.money() >= ferryPrice){
                GameState = LAKESELECT;
                islandEnabled = true;
                for (unsigned i = 0; i < lakes[0].spots.count(); i++)
                    lakes[0].spots[i].locked = true;
                lakes[0].spots[6].locked = false;
                lakes[0].spots[5].locked = false;

                me.setMoney(me.money() - ferryPrice);
                ss.playsound(2, false);
            }
            else{
                strcpy(boatMan[0], boatManSays1[0]);
                strcpy(boatMan[1], boatManSays1[1]);
                strcpy(boatMan[2], boatManSays1[2]);
            }
        }
        
        if (boatNo_button.isPointerOnTop(mx, my)){
            GameState = LAKESELECT;
            enterLake();
            strcpy(boatMan[0], boatManSays[0]);
            strcpy(boatMan[1], boatManSays[1]);
            strcpy(boatMan[2], boatManSays[2]);
        }
        
        if (boatExit_button.isPointerOnTop(mx, my)){
            GameState = LAKESELECT;
            strcpy(boatMan[0], boatManSays[0]);
            strcpy(boatMan[1], boatManSays[1]);
            strcpy(boatMan[2], boatManSays[2]);
        }
    }
}

//-------------------
void Singleton::render(){
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        glViewport(0, 0, 320, 480);
        glRotatef(-90, 0, 0, 1);
        glOrthof(0.0, (GLfloat) 480, (GLfloat) 320, 0.0, 400, -400);
    #else
        glOrtho(0.0, (GLfloat) 480, (GLfloat) 320, 0.0, 400, -400);
    #endif
    
    switch(GameState){
        case INTRO:draw_intro(); break;
        case TOWN: DrawAdventureMode(SceneIndex, false); break;
        case LAKE: DrawLake(); break;
        case STORE: DrawShop(); break;
        case CREDITS: DrawCredits(); break;
        case HELP: DrawHelp(); break;
        case SELLPOINT: DrawRestaurant(); break;
        case OPTIONS: drawOptions(); break;
        case LAKESELECT: drawFishingSpotSelection(); break;
        case BOAT:drawBoat(); break;
        case BUS: drawBus(); break;
        case CAT: drawCat(); break;
    }

    /*char buf[100];
    sprintf(buf, "useLineTension %d", sys.useLineTension);
    WriteShadedText(10, 10, pics, 0, buf);*/
    
#if !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
    if (!windowed)
        pics.draw(19, MouseX, MouseY, 2, false);
#endif

    glEnable(GL_TEXTURE_2D);
    pics.drawBatch(666);   
}
//---------------------
void Singleton::logic(){
    
    if (music.playing())
        music.update();
    
    if (touches.down.count()){
        MouseX = touches.down[0].v[0];
        MouseY = touches.down[0].v[1];
    }
    
    if (touches.move.count()){
        MouseX = touches.move[0].v[0];
        MouseY = touches.move[0].v[1];
    }
   
    switch(GameState){
        case INTRO: mainMenuLogic(); break;
        case TOWN: AdventureLogic(); break;
        case LAKE: LakeLogic(); break;
        case STORE: ShopLogic(); break;
        case CREDITS: Credits(); break;
        case HELP: Help(); break;
        case SELLPOINT: punktas(); break;
        case OPTIONS: optionsLogic(); break;
        case LAKESELECT: fishingSpotSelectionLogic(); break;
        case BOAT: boatLogic(); break;
        case BUS: busLogic(); break;
        case CAT: catLogic(); break;
    }   
    touches.oldDown.destroy();
    for (unsigned long i = 0; i < touches.down.count(); i++ ){
        Vector3D v = touches.down[i];
        touches.oldDown.add(v);
    }
    touches.up.destroy();
    touches.down.destroy();
    touches.move.destroy();
}
//-------------------------
void Singleton::destroy(){
    puts("Bye");
    fishData.destroy();
    baitData.destroy();
    fishes.destroy();
    me.destroy();
    music.release();
    ss.freeData();
    ss.exit();
    pics.destroy();
    LakeGfx.destroy();
    walkPath.destroy();
    for (unsigned i = 0; i < Scenes.count(); i++)
        Scenes[i].destroy();
    Scenes.destroy();
    lakes[0].destroy();
    lakes[1].destroy();
}


