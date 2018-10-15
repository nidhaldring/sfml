#include"audio.h"


std::string Audio::location="resources/sounds/";
sf::Music Audio::theme;
sf::SoundBuffer Audio::laser;
sf::Sound Audio::laser_effect;
sf::SoundBuffer Audio::explosion;
sf::Sound Audio::explosion_effect;
sf::SoundBuffer Audio::teleport;
sf::Sound Audio::teleport_effect;

void Audio::init(){
	theme.openFromFile(location+"theme.wav");
	theme.setLoop(true);
	theme.play();
	laser.loadFromFile(location+"laser.wav");
	laser_effect.setBuffer(laser);
	explosion.loadFromFile(location+"explosion.wav");
	explosion_effect.setBuffer(explosion);
	teleport.loadFromFile(location+"teleport.wav");
	teleport_effect.setBuffer(teleport);
}

void Audio::playHyperEffect(){
	if(teleport_effect.getStatus() != sf::SoundSource::Status::Playing)
		teleport_effect.play();
}

