#ifndef ENTITIES_H
#define ENTITIES_H
#include<SFML/Graphics.hpp>
#include"entity.h"
#include"entities.h"
using namespace sf;
using namespace std;

class Game;
extern Game game;

class Enemy:public Entity{
public:
	Enemy();
	virtual ~Enemy()=default;
};

// metero

class Metero:public Enemy{
private:
	Texture& metero_txt;
	Sprite& metero;
public:
	Metero(float=5.f);
	void fall();
	void multiply();
	//void fire(Game&) const;
	virtual void update() override;
	virtual const Vector2f& getForce() const {
		return speed;
	}
	virtual void applyForce(const Vector2f&) ;
	virtual void onCollide(Entity*);
	virtual  ~Metero()=default;

};

//ship

class Ship:public Entity{
private:
	Sprite& ship;
	Texture& ship_txt;
	float angle=0.f;
public:
	Ship();
	Ship(const Ship&)=delete;
	Ship& operator=(const Ship&)=delete;
	virtual void update() override;
	void jumpToHyperSpace();
	void fire(Game&) ;
	virtual ~Ship()=default;
};

// shot

class Bullet:public Entity {
private:
	Sprite& bullet;
	Texture& bullet_txt;
	Vector2f force;
public:
	Bullet(const Entity*);
	virtual void update() override;
	virtual void onDestroy() override{
		dead=true;
	}
};

#endif
