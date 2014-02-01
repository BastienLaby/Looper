#include "fmod.hpp" //FMOD Ex
#include "fmod_errors.h" //FMOD Ex
#include <iostream>
#include <cstdlib>
#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


#ifndef _SOUND_H
#define _SOUND_H


/*************************************************************************************************/
/*************************************************************************************************/
/******************************************	COMMON	**********************************************/
/*************************************************************************************************/
/*************************************************************************************************/

#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#define _kbhit kbhit
#define _getch getch
#define stricmp strcasecmp
#define strnicmp strncasecmp

#define Sleep(x) usleep((x)*1000)

static int    inited=0;
static struct termios ori;

static void tcatexit(){
   tcsetattr(0,TCSANOW,&ori);
}

static void init_terminal(){
   struct termios t;
   tcgetattr(0,&t);
   tcgetattr(0,&ori);
   t.c_lflag &= ~(ICANON);
   t.c_lflag &= ~(ECHO);
   tcsetattr(0,TCSANOW,&t);
   atexit(tcatexit);
}

static inline int kbhit(){
  fd_set rfds;
  struct timeval tv;

   if (!inited){
	  inited=1;
	  init_terminal();
   }

   FD_ZERO(&rfds);
   FD_SET(0, &rfds);
   tv.tv_sec = 0;
   tv.tv_usec = 10*1000;
   return select(1, &rfds, NULL, NULL, &tv)>0;
}

static inline int getch(){
   fd_set rfds;

   if (!inited){
	  inited=1;
	  init_terminal();
   }

   FD_ZERO(&rfds);
   FD_SET(0, &rfds);
   if (select(1, &rfds, NULL, NULL, NULL)>0)
	 return getchar();
   else{
	  printf("wincompat.h: select() on fd 0 failed\n");
	  return 0xDeadBeef;
   }
}


void ERRCHECK(FMOD_RESULT result);


/*************************************************************************************************/
/*************************************************************************************************/
/************************************	CLASS DEFINITION	**************************************/
/*************************************************************************************************/
/*************************************************************************************************/

namespace sound{

class SoundPlayer {
private:
    FMOD::System* system;
    std::vector<FMOD::Sound *> sounds;
    static FMOD::Channel *channel;
    unsigned int version;

public:
    FMOD_RESULT result;

    static unsigned int ms;
    static unsigned int lenms;
    static bool playing;
    static bool paused;
    static int channelsplaying;

    SoundPlayer(void);
    ~SoundPlayer(void);

    size_t loadSound(const char* directory);
    void loadFromFolder(const char* filename);
    void play(size_t index);

    void updateState(void);

    // setters
    void loop(size_t index);
    void removeLoop(size_t index);
    void toggleLoop(size_t index);

    // getters
    size_t getSoundsCount(void);

    inline void errCheck(void) {	ERRCHECK(result);};

};

}

#endif // _SOUND_H
