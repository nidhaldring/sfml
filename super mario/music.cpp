#include "music.h"

// music and sound objects 
Music MainTheme;
SoundBuffer DeathThemeBuffer;
Sound DeathTheme;
SoundBuffer OutOfTimeBuffer;
Sound OutOfTimeTheme;
Music SpedUpMainTheme;
SoundBuffer JumpingThemeBuffer;
Sound JumpingTheme;

// methods to control and get status
void loadMusic(){
		assert(MainTheme.openFromFile("resources/audio/main_theme.ogg"));
		assert(DeathThemeBuffer.loadFromFile("resources/audio/death.wav"));
		assert(OutOfTimeBuffer.loadFromFile("resources/audio/out_of_time.wav"));
		assert(SpedUpMainTheme.openFromFile("resources/audio/main_theme_sped_up.ogg"));
		assert(JumpingThemeBuffer.loadFromFile("resources/audio/maro-jump-sound-effect.ogg"));
		JumpingTheme.setBuffer(JumpingThemeBuffer);
		DeathTheme.setBuffer(DeathThemeBuffer);
		OutOfTimeTheme.setBuffer(OutOfTimeBuffer);
	}

void playMainTheme(){
	MainTheme.setLoop(true);
	MainTheme.play();
}	

void playDeathTheme(){
	MainTheme.stop();
	OutOfTimeTheme.stop();
	SpedUpMainTheme.stop();
	DeathTheme.play();
}

void playOutOfTimeTheme(){
	MainTheme.stop();
	SpedUpMainTheme.stop();
	OutOfTimeTheme.play();
}

bool DeathMusicEnded(){
	return DeathTheme.getStatus() == SoundSource::Status::Stopped ;
}

bool OutOfTimeThemeEnded(){
	return OutOfTimeTheme.getStatus() == SoundSource::Status::Stopped;
}

void playSpedUpMainTheme(){
	if(SpedUpMainTheme.getStatus() == SoundSource::Status::Playing)
		return;
	MainTheme.stop();
	SpedUpMainTheme.setLoop(true);
	SpedUpMainTheme.play();
}

bool SpedUpMainThemePlaying(){
	return SpedUpMainTheme.getStatus() == SoundSource::Status::Playing;
}
void playJumpingTheme(){
	if(JumpingTheme.getStatus() == SoundSource::Status::Playing)
		return;
	JumpingTheme.play();
}