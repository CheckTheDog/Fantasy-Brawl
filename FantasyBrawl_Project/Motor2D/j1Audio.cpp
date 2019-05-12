#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Audio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.assign("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	LoadAudio(config);

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	std::list<Mix_Chunk*>::const_iterator item = fx.begin();

	for(; item != fx.end(); item++)
		Mix_FreeChunk(*item);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		std::list <Mix_Chunk*>::const_iterator it;
		it = std::next(fx.begin(), id - 1);
		Mix_PlayChannel(-1, *it, repeat);
	}

	return ret;
}

int j1Audio::getMusicVolume() const
{
	return Mix_VolumeMusic(-1);
}

//int j1Audio::getFxVolume() const
//{
//	return fx_volume;
//}

void j1Audio::setMusicVolume(float volume)
{
	Mix_VolumeMusic(MIX_MAX_VOLUME*volume);
	music_volume = MIX_MAX_VOLUME * volume; //Save it for fading;
}

//void j1Audio::setFxVolume(float volume)
//{
//	for (p2List_item<Mix_Chunk*>* item = fx.start; item; item = item->next)
//	{
//		Mix_VolumeChunk(item->data, MIX_MAX_VOLUME*volume);
//	}
//	fx_volume = MIX_MAX_VOLUME * volume; //Save it for future loaded fx
//} 

void j1Audio::LoadAudio(pugi::xml_node& config)
{
	//FX Loads
	fxfolder = config.child("fx").child_value("folder");

	pugi::xml_node sound_node = config.child("fx").child("sound");

	pathCursor = sound_node.attribute("cursor").as_string();
	std::string cursorSound = fxfolder + pathCursor;
	fxCursor = LoadFx(cursorSound.data());

	pathConfirm = sound_node.attribute("confirm").as_string();
	std::string confirmSound = fxfolder + pathConfirm;
	fxConfirm = LoadFx(confirmSound.data());

	pathCancel = sound_node.attribute("cancel").as_string();
	std::string cancelSound = fxfolder + pathCancel;
	fxCancel = LoadFx(cancelSound.data());

	pathScroll = sound_node.attribute("scroll").as_string();
	std::string scrollSound = fxfolder + pathScroll;
	fxScroll = LoadFx(scrollSound.data());

	pathCursor2 = sound_node.attribute("cursor2").as_string();
	std::string cursor2Sound = fxfolder + pathCursor2;
	fxCursor2 = LoadFx(cursor2Sound.data());

	pathConfirmChamp = sound_node.attribute("confirmchamp").as_string();
	std::string confirmchampSound = fxfolder + pathConfirmChamp;
	fxConfirmChamp = LoadFx(confirmchampSound.data());

	pathCancelChamp = sound_node.attribute("cancelchamp").as_string();
	std::string cancelchampSound = fxfolder + pathCancelChamp;
	fxCancelChamp = LoadFx(cancelchampSound.data());

	pathCountdown = sound_node.attribute("countdown").as_string();
	std::string countdownSound = fxfolder + pathCountdown;
	fxCountdown = LoadFx(countdownSound.data());

	pathBrawlStart = sound_node.attribute("brawlstart").as_string();
	std::string brawlstartSound = fxfolder + pathBrawlStart;
	fxBrawlStart = LoadFx(brawlstartSound.data());

	pathTraktBasic = sound_node.attribute("traktbasic").as_string();
	std::string traktbasicSound = fxfolder + pathTraktBasic;
	fxTraktBasic = LoadFx(traktbasicSound.data());

	pathTraktSpecial = sound_node.attribute("traktspecial").as_string();
	std::string traktspecialSound = fxfolder + pathTraktSpecial;
	fxTraktSpecial = LoadFx(traktspecialSound.data());

	pathTraktDeath = sound_node.attribute("traktdeath").as_string();
	std::string traktdeathSound = fxfolder + pathTraktDeath;
	fxTraktDeath = LoadFx(traktdeathSound.data());

	pathWendolinBasic = sound_node.attribute("wendolinbasic").as_string();
	std::string wendolinbasicSound = fxfolder + pathWendolinBasic;
	fxWendolinBasic = LoadFx(wendolinbasicSound.data());

	pathWendolinSpecial = sound_node.attribute("wendolinspecial").as_string();
	std::string wendolinspecialSound = fxfolder + pathWendolinSpecial;
	fxWendolinSpecial = LoadFx(wendolinspecialSound.data());

	pathWendolinDeath = sound_node.attribute("wendolindeath").as_string();
	std::string wendolindeathSound = fxfolder + pathWendolinDeath;
	fxWendolinDeath = LoadFx(wendolindeathSound.data());

	pathSimonBasic = sound_node.attribute("simonbasic").as_string();
	std::string simonbasicSound = fxfolder + pathSimonBasic;
	fxSimonBasic = LoadFx(simonbasicSound.data());

	pathSimonSpecial = sound_node.attribute("simonspecial").as_string();
	std::string simonspecialSound = fxfolder + pathSimonSpecial;
	fxSimonSpecial = LoadFx(simonspecialSound.data());

	pathSimonDeath = sound_node.attribute("simondeath").as_string();
	std::string simondeathSound = fxfolder + pathSimonDeath;
	fxSimonDeath = LoadFx(simondeathSound.data());

	pathMeliadoulBasic = sound_node.attribute("meliadoulbasic").as_string();
	std::string meliadoulbasicSound = fxfolder + pathMeliadoulBasic;
	fxMeliadoulBasic = LoadFx(meliadoulbasicSound.data());

	pathMeliadoulSpecial = sound_node.attribute("meliadoulspecial").as_string();
	std::string meliadoulspecialSound = fxfolder + pathMeliadoulSpecial;
	fxMeliadoulSpecial = LoadFx(meliadoulspecialSound.data());

	pathMeliadoulDeath = sound_node.attribute("meliadouldeath").as_string();
	std::string meliadouldeathSound = fxfolder + pathMeliadoulDeath;
	fxMeliadoulDeath = LoadFx(meliadouldeathSound.data());

	pathSpecialAvailable = sound_node.attribute("specialavailable").as_string();
	std::string specialavailableSound = fxfolder + pathSpecialAvailable;
	fxSpecialAvailable = LoadFx(specialavailableSound.data());

	pathHit1 = sound_node.attribute("hit1").as_string();
	std::string hit1Sound = fxfolder + pathHit1;
	fxHit1 = LoadFx(hit1Sound.data());

	pathHit2 = sound_node.attribute("hit2").as_string();
	std::string hit2Sound = fxfolder + pathHit2;
	fxHit2 = LoadFx(hit2Sound.data());

	pathHit3 = sound_node.attribute("hit3").as_string();
	std::string hit3Sound = fxfolder + pathHit3;
	fxHit3 = LoadFx(hit3Sound.data());

	pathShieldHit1 = sound_node.attribute("shieldhit1").as_string();
	std::string shieldhit1Sound = fxfolder + pathShieldHit1;
	fxShieldHit1 = LoadFx(shieldhit1Sound.data());

	pathShieldHit2 = sound_node.attribute("shieldhit2").as_string();
	std::string shieldhit2Sound = fxfolder + pathShieldHit2;
	fxShieldHit2 = LoadFx(shieldhit2Sound.data());

	pathPowerUpAppear1 = sound_node.attribute("powerup1").as_string();
	std::string powerupappear1Sound = fxfolder + pathPowerUpAppear1;
	fxPowerUpAppear1 = LoadFx(powerupappear1Sound.data());

	pathPowerUpAppear2 = sound_node.attribute("powerup2").as_string();
	std::string powerupappear2Sound = fxfolder + pathPowerUpAppear2;
	fxPowerUpAppear2 = LoadFx(powerupappear2Sound.data());

	pathPowerUpAppear3 = sound_node.attribute("powerup3").as_string();
	std::string powerupappear3Sound = fxfolder + pathPowerUpAppear3;
	fxPowerUpAppear3 = LoadFx(powerupappear3Sound.data());

	pathPowerUpPick = sound_node.attribute("poweruppick").as_string();
	std::string poweruppickSound = fxfolder + pathPowerUpPick;
	fxPowerUpPick = LoadFx(poweruppickSound.data());

	pathPowerUpActivate = sound_node.attribute("powerupactivation").as_string();
	std::string powerupactivateSound = fxfolder + pathPowerUpActivate;
	fxPowerUpActivate = LoadFx(powerupactivateSound.data());

	pathStormCloseCount = sound_node.attribute("stormcount").as_string();
	std::string stormclosecountSound = fxfolder + pathStormCloseCount;
	fxStormCloseCount = LoadFx(stormclosecountSound.data());

	pathStormClose = sound_node.attribute("stormclose").as_string();
	std::string stormcloseSound = fxfolder + pathStormClose;
	fxStormClose = LoadFx(stormcloseSound.data());

	pathPause = sound_node.attribute("pause").as_string();
	std::string pauseSound = fxfolder + pathPause;
	fxPause = LoadFx(pauseSound.data());

	pathRoundEnd = sound_node.attribute("roundend").as_string();
	std::string roundendSound = fxfolder + pathRoundEnd;
	fxRoundEnd = LoadFx(roundendSound.data());

	pathMatchEnd = sound_node.attribute("matchend").as_string();
	std::string matchendSound = fxfolder + pathMatchEnd;
	fxMatchEnd = LoadFx(matchendSound.data());

	//Music Loads
	musicfolder = config.child("music").child_value("folder");

	pugi::xml_node music_node = config.child("music").child("song");

	pathIntro = music_node.attribute("name").as_string();
	pathIntro = musicfolder + pathIntro;
	music_node = music_node.next_sibling();

	pathMainMenu1 = music_node.attribute("name").as_string();
	pathMainMenu1 = musicfolder + pathMainMenu1;
	music_node = music_node.next_sibling();

	pathMainMenu2 = music_node.attribute("name").as_string();
	pathMainMenu2 = musicfolder + pathMainMenu2;
	music_node = music_node.next_sibling();

	pathChampSelect = music_node.attribute("name").as_string();
	pathChampSelect = musicfolder + pathChampSelect;
	music_node = music_node.next_sibling();

	pathGallery = music_node.attribute("name").as_string();
	pathGallery = musicfolder + pathGallery;
	music_node = music_node.next_sibling();

	pathOptions = music_node.attribute("name").as_string();
	pathOptions = musicfolder + pathOptions;
	music_node = music_node.next_sibling();

	pathLoadScreen1 = music_node.attribute("name").as_string();
	pathLoadScreen1 = musicfolder + pathLoadScreen1;
	music_node = music_node.next_sibling();

	pathLoadScreen2 = music_node.attribute("name").as_string();
	pathLoadScreen2 = musicfolder + pathLoadScreen2;
	music_node = music_node.next_sibling();

	pathLoadScreen3 = music_node.attribute("name").as_string();
	pathLoadScreen3 = musicfolder + pathLoadScreen3;
	music_node = music_node.next_sibling();

	pathLoadScreen4 = music_node.attribute("name").as_string();
	pathLoadScreen4 = musicfolder + pathLoadScreen4;
	music_node = music_node.next_sibling();

	pathMap1 = music_node.attribute("name").as_string();
	pathMap1 = musicfolder + pathMap1;
	music_node = music_node.next_sibling();

	pathMap2_1 = music_node.attribute("name").as_string();
	pathMap2_1 = musicfolder + pathMap2_1;
	music_node = music_node.next_sibling();

	pathMap2_2 = music_node.attribute("name").as_string();
	pathMap2_2 = musicfolder + pathMap2_2;
	music_node = music_node.next_sibling();

	pathMap3 = music_node.attribute("name").as_string();
	pathMap3 = musicfolder + pathMap3;
	music_node = music_node.next_sibling();

	pathMap4 = music_node.attribute("name").as_string();
	pathMap4 = musicfolder + pathMap4;
	music_node = music_node.next_sibling();

	pathLeaderBoard = music_node.attribute("name").as_string();
	pathLeaderBoard = musicfolder + pathLeaderBoard;
	music_node = music_node.next_sibling();

}