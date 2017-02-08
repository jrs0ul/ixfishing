#import "EAGLView.h"

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;



// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}
//------------------------------------
//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
        
        strncpy(cppstuff.DocumentPath, [[NSSearchPathForDirectoriesInDomains( NSDocumentDirectory
                                                                             , NSUserDomainMask
                                                                             , YES) objectAtIndex:0] cString], 255);
        
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffersOES(1, &defaultFramebuffer);
        glGenRenderbuffersOES(1, &colorRenderbuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);   
        
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;
        
        //---
        cppstuff.init();
        
        self.multipleTouchEnabled = YES;
        
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / 60)];
        [[UIAccelerometer sharedAccelerometer] setDelegate:self];
        

        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
    }

    return self;
}

//--------------------------------------------------------------
- (void)drawView:(id)sender{
       
    
    [EAGLContext setCurrentContext:context];
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    cppstuff.render();
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
    //------------------------------
    cppstuff.logic();
    
    if (cppstuff.launchjrs0ul){
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"http://jrs0ul.com/en/projects/"]];
        cppstuff.launchjrs0ul = false;
    }
    
    if (cppstuff.launchmystore){
        cppstuff.saveMyData();
        
        NSString *appstoreLink = @"itms-apps://itunes.com/apps/jrs0ul";
        
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:appstoreLink]];
        
        cppstuff.launchmystore = false;
    }

}
//--------------------------------------------------
- (void)layoutSubviews{
    
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer *)self.layer];
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
    
    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    
    [self drawView:nil];
}
//----------------------------------------
- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}
//----------------------------------------
- (void)setAnimationFrameInterval:(NSInteger)frameInterval{
      if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}
//------------------------------------
- (void)startAnimation{
    if (!animating){
        
        if (displayLinkSupported){
            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
        if (!cppstuff.NoMusic_Button.state)
            cppstuff.music.playback();
    }
}
//----------------------------------------
- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
        if (!cppstuff.NoMusic_Button.state)
            cppstuff.music.stop();
        cppstuff.saveMyData();
    }
}
//---------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
       
    cppstuff.touches.down.destroy();
    if ([touches count])
        cppstuff.touches.allfingersup = false;
    for (int i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].y, 
                   320 - [t locationInView:self].x, 0);
        cppstuff.touches.down.add(v);
    }
   }
//------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.move.destroy();
    if ([touches count])
        cppstuff.touches.allfingersup = false;
    for (int i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].y, 
                   320 - [t locationInView:self].x, 0);
        cppstuff.touches.move.add(v);
    }
}
//------------------------------------

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.up.destroy();
    for (int i = 0; i < [touches count]; i++){
        UITouch *t = [[touches allObjects] objectAtIndex:i];
        Vector3D v([t locationInView:self].y, 
                   320 - [t locationInView:self].x, 0);
        cppstuff.touches.up.add(v);
    }
    //----
    if ([touches count] == [[event touchesForView:self] count]) //all fingers up
        cppstuff.touches.allfingersup = true;
   
    
}
//------------------------------------

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    cppstuff.touches.up.destroy();
    cppstuff.touches.move.destroy();
    cppstuff.touches.down.destroy();
   
    cppstuff.touches.allfingersup = true;
  
}
//------------------------------------
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration{
    
    float kFilteringFactor = 0.8f;
    
    cppstuff._acceleration.v[0] = acceleration.x * kFilteringFactor + cppstuff._acceleration.v[0] * (1.0 - kFilteringFactor);
    cppstuff._acceleration.v[1] = acceleration.y * kFilteringFactor + cppstuff._acceleration.v[1] * (1.0 - kFilteringFactor);
    cppstuff._acceleration.v[2] = acceleration.z * kFilteringFactor + cppstuff._acceleration.v[2] * (1.0 - kFilteringFactor);
    
    
}

//---------------------------------------
- (void)dealloc{
    puts("bye");
    // Tear down GL
    if (defaultFramebuffer){
        glDeleteFramebuffersOES(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
    
    if (colorRenderbuffer){
        glDeleteRenderbuffersOES(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    cppstuff.saveMyData();
    
    cppstuff.destroy();
    
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    context = nil;
    [super dealloc];
}

@end
