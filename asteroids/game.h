#ifndef GAME_H
#define GAME_H
#include<list>
#include"entity.h"
#include"entities.h"
#include<random>
#include<cmath>
using namespace std;
using namespace sf;

class Game{	
private:

	RenderWindow w;
	list<Entity*> entities;
	short lvl=0,score=0;
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
	void run();
	void addEntity(Entity*);
	~Game();
};


#endif