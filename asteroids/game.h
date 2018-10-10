#ifndef GAME_H
#define GAME_H
#include<SFML/Window.hpp>
#include<list>
#include"entities.h"
#include<random>
#include<cmath>
using namespace std;
class Game{
	RenderWindow w;
	list<Entity*> entities;
	short lvl=0;
	short score=0;
	Ship* ship;
	random_device rd;
	Sprite background1,background2;
	Texture back_txt1,back_txt2;
	void loadBackground();
	void createLvl();
	void renderEntities();
	void updateBackground();
	void renderBackground();
	void moveBackground(Sprite&);
	void checkCollosion() const ;
public:
	Game();
	Game(const Game&)=delete;
	Game& operator=(const Game&)=delete;
	~Game();
	void run();
};


#endif