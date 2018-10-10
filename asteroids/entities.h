#ifndef ENTITIES_H
#define ENTITIES_H
#include<SFML/Graphics.hpp>
#include"entity.h"
using namespace sf;
using namespace std;

class Enemy:public Entity{
public:
	Enemy();
	virtual ~Enemy()=default;
};

// metero

class Metero:public Enemy{

	Texture& metero_txt;
	Sprite& metero;
public:
	Metero(float=5.f);
	void fall();
	void multiply();
	virtual void update() override;
	virtual  ~Metero()=default;

};

//ship

class Ship:public Entity{
	Sprite& ship;
	Texture& ship_txt;
	float angle=0.f;
public:
	Ship();
	Ship(const Ship&)=delete;
	Ship& operator=(const Ship&)=delete;
	virtual void update() override;
	void jumpToHyperSpace();
	virtual ~Ship()=default;
};

#endif
