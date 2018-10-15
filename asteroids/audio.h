#include<SFML/Audio.hpp>
#include<string>

struct Audio{
static sf::Music theme;
static sf::SoundBuffer laser;
static sf::Sound laser_effect;
static sf::SoundBuffer explosion;
static sf::Sound explosion_effect;
static sf::SoundBuffer teleport;
static sf::Sound teleport_effect;
static std::string location;

static void init();
static void playHyperEffect();

Audio()=delete;

};

