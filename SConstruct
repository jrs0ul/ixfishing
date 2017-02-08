import os
import sys

OBJ_DIR = os.path.join(os.getcwd(), 'obj/');
env = Environment(ENV=os.environ,
                  CCFLAGS=['-O3','-Wall'],
                  OBJPREFIX = OBJ_DIR)


code = [
	"src/Baits.cpp",
	"src/gui/Button.cpp",
	"src/Collisions.cpp",
	"src/CppSingleton.cpp",
	"src/Extensions.cpp",
	"src/externals.cpp",
	"src/fCommon.cpp",
	"src/FindPath.cpp",
	"src/Fish.cpp",
	"src/Image.cpp",
	"src/Lake.cpp",
	"src/mainer.cpp",
	"src/Matrix.cpp",
	"src/MyData.cpp",
	"src/audio/OggStream.cpp",
	"src/gui/Scrollbar.cpp",
	"src/SDLVideo.cpp",
	"src/audio/SoundSystem.cpp",
	"src/SysConfig.cpp",
	"src/gui/Text.cpp",
	"src/TextureLoader.cpp",
	"src/Threads.cpp",
	"src/Utils.cpp",
	"src/Vectors.cpp",
	"src/Xml.cpp",
	"src/gui/YesNoDialog.cpp"

        ]

#Windblows
if sys.platform == 'win32':
    Tool('mingw')(env)
    env.Append(LIBS = ['mingw32','SDLmain', 'SDL', 'OpenGL32',
                       'openal32', 'vorbisfile', 'ogg'])
else: #Mac OS X
    if sys.platform == 'darwin':
        env['FRAMEWORKS'] = ['Cocoa', 'SDL', 'OPENGL', 'OpenAL','vorbis', 'Ogg', 'libcurl']
        env.Append(LIBS = ['iconv', 'pthread'])
        env.Append(CCFLAGS = ['-arch', 'i386'])
        env.Object('src/osx/SDLMain.m', OBJPREFIX = OBJ_DIR,
                   CCFLAGS = ['-arch', 'i386', '-I/Library/Frameworks/SDL.framework/Headers/'])
        code.append('obj/SDLMain.o')
        env.Append(LINKFLAGS = ['-arch', 'i386'])
    else: #Linux
        env.Append(LIBS = ['SDL','GL', 'openal', 'vorbisfile',
                           'ogg', 'pthread', 'libcurl'])


env.Program(target = "IXFishing", 
            source = code
            )

