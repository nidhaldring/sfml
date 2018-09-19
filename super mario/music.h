#ifndef MUSIC_H
#define MUSIC_H
#include<cassert>
#include<SFML/Audio.hpp>
using namespace sf;

void loadMusic();
void playMainTheme();	
void playDeathTheme();
bool DeathMusicEnded();
void playOutOfTimeTheme();
bool OutOfTimeThemeEnded();
bool SpedUpMainThemePlaying();
void playSpedUpMainTheme();
void playJumpingTheme();

#endif

