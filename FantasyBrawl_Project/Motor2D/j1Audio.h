#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void LoadAudio(pugi::xml_node&);

	std::string		musicfolder;
	std::string		fxfolder;

	std::string		pathIntro;
	std::string		pathMainMenu1;
	std::string		pathMainMenu2;
	std::string		pathChampSelect;
	std::string		pathGallery;
	std::string		pathOptions;
	std::string		pathLoadScreen1;
	std::string		pathLoadScreen2;
	std::string		pathLoadScreen3;
	std::string		pathLoadScreen4;
	std::string		pathMap1;
	std::string		pathMap2_1;
	std::string		pathMap2_2;
	std::string		pathMap3;
	std::string		pathMap4;
	std::string		pathLeaderBoard;

	std::string		pathCursor;
	std::string		pathConfirm;
	std::string		pathCancel;
	std::string		pathScroll;
	std::string		pathCursor2;
	std::string		pathConfirmChamp;
	std::string		pathCancelChamp;
	std::string		pathCountdown;
	std::string		pathBrawlStart;
	std::string		pathTraktBasic;
	std::string		pathTraktSpecial;
	std::string		pathTraktDeath;
	std::string		pathWendolinBasic;
	std::string		pathWendolinSpecial;
	std::string		pathWendolinDeath;
	std::string		pathSimonBasic;
	std::string		pathSimonSpecial;
	std::string		pathSimonDeath;
	std::string		pathMeliadoulBasic;
	std::string		pathMeliadoulSpecial;
	std::string		pathMeliadoulDeath;
	std::string		pathSpecialAvailable;
	std::string		pathHit1;
	std::string		pathHit2;
	std::string		pathHit3;
	std::string		pathShieldHit2;
	std::string		pathShieldHit1;
	std::string		pathPowerUpAppear1;
	std::string		pathPowerUpAppear2;
	std::string		pathPowerUpAppear3;
	std::string		pathPowerUpPick;
	std::string		pathPowerUpActivate;
	std::string		pathStormCloseCount;
	std::string		pathStormClose;
	std::string		pathPause;
	std::string		pathRoundEnd;
	std::string		pathMatchEnd;

	uint	fxCursor = NULL;
	uint	fxConfirm = NULL;
	uint	fxCancel = NULL;
	uint	fxScroll = NULL;
	uint	fxCursor2 = NULL;
	uint	fxConfirmChamp = NULL;
	uint	fxCancelChamp = NULL;
	uint	fxCountdown = NULL;
	uint	fxBrawlStart = NULL;
	uint	fxTraktBasic = NULL;
	uint	fxTraktSpecial = NULL;
	uint	fxTraktDeath = NULL;
	uint	fxWendolinBasic = NULL;
	uint	fxWendolinSpecial = NULL;
	uint	fxWendolinDeath = NULL;
	uint	fxSimonBasic = NULL;
	uint	fxSimonSpecial = NULL;
	uint	fxSimonDeath = NULL;
	uint	fxMeliadoulBasic = NULL;
	uint	fxMeliadoulSpecial = NULL;
	uint	fxMeliadoulDeath = NULL;
	uint	fxSpecialAvailable = NULL;
	uint	fxHit1 = NULL;
	uint	fxHit2 = NULL;
	uint	fxHit3 = NULL;
	uint	fxShieldHit2 = NULL;
	uint	fxShieldHit1 = NULL;
	uint	fxPowerUpAppear1 = NULL;
	uint	fxPowerUpAppear2 = NULL;
	uint	fxPowerUpAppear3 = NULL;
	uint	fxPowerUpPick = NULL;
	uint	fxPowerUpActivate = NULL;
	uint	fxStormCloseCount = NULL;
	uint	fxStormClose = NULL;
	uint	fxPause = NULL;
	uint	fxRoundEnd = NULL;
	uint	fxMatchEnd = NULL;

private:

	_Mix_Music*			music = NULL;
	std::list <Mix_Chunk*>	fx;


};

#endif // __j1AUDIO_H__