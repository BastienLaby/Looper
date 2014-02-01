#include "sound/SoundPlayer.hpp"
#include <iterator>
#include <vector>
#include "fouch/Timer.hpp"



void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cerr<<"FMOD error! ("<<result<<") "<<FMOD_ErrorString(result)<<std::endl;
        exit(0);
    }
}


namespace sound{

/******** CLASS VARIABLE DECLARATIONS ********/

FMOD::Channel *SoundPlayer::channel = 0;

unsigned int SoundPlayer::ms = 0;
unsigned int SoundPlayer::lenms = 0;
bool SoundPlayer::playing = 0;
bool SoundPlayer::paused = 0;
int SoundPlayer::channelsplaying = 0;

/******** METHODS' IMPLEMENTATIONS ********/

// Constructor :  Check FMOD version and init it
SoundPlayer::SoundPlayer() {
	result = FMOD::System_Create(&system);
	errCheck();

    result = system->getVersion(&version);
    errCheck();

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        getch();
        exit(-1);
    }

    result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    errCheck();

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    errCheck();
}


// Load every sound before playing them
size_t SoundPlayer::loadSound(const char * filename){

	size_t index = sounds.size();
	FMOD::Sound * sound;
	sounds.push_back(sound);

    result = system->createSound(filename, FMOD_SOFTWARE, 0, &sounds[index]);
    errCheck();
    result = sounds[index]->setMode(FMOD_LOOP_OFF);
    errCheck();

    return index;
}


void SoundPlayer::loadFromFolder(const char* directory){

	struct dirent *lecture;
	DIR *rep;
	rep = opendir(directory);

	while ((lecture = readdir(rep))) {
		std::string musicName = lecture->d_name;
		std::string fullName = directory;
		if(musicName.find(".") != 0 && lecture->d_type != DT_DIR){
			fullName.append("/").append(musicName);
			std::cerr<<"Loading sound "<<lecture->d_name<<" ... "<<std::endl;
			loadSound(fullName.c_str());
			std::cerr<<"DONE !"<<std::endl;
		}
	}

	closedir(rep);

}


void SoundPlayer::play(size_t index){
    result = system->playSound(FMOD_CHANNEL_FREE, sounds[index], 0, &channel);
    errCheck();
}



void SoundPlayer::updateState(void){
	system->update();

	if (channel)
	{
		FMOD::Sound *currentsound = 0;

		result = channel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			errCheck();
		}

		result = channel->getPaused(&paused);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			errCheck();
		}

		result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			errCheck();
		}

		channel->getCurrentSound(&currentsound);
		if (currentsound)
		{
			result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
			if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
			{
				errCheck();
			}
		}
	}

	system->getChannelsPlaying(&channelsplaying);

}



/******** SETTERS *******/
void SoundPlayer::loop(size_t index){
	if(index > sounds.size())	exit(-1);
    result = sounds[index]->setMode(FMOD_LOOP_NORMAL);
    errCheck();
}

void SoundPlayer::removeLoop(size_t index){
	if(index > sounds.size())	exit(-1);
    result = sounds[index]->setMode(FMOD_LOOP_OFF);
    errCheck();
}

void SoundPlayer::toggleLoop(size_t index){
	if(index > sounds.size())	exit(-1);
    FMOD_MODE mode;
    sounds[index]->getMode(&mode);
    if(mode == FMOD_LOOP_OFF)	loop(index);
    else						removeLoop(index);
}


// Destructor
SoundPlayer::~SoundPlayer(void){
	for(size_t i=0; i < sounds.size(); ++i){
		result = sounds[i]->release();
		errCheck();
	}
	result = system->close();
	errCheck();
	result = system->release();
	errCheck();
}

}
