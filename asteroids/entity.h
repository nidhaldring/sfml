#ifndef ENTITY_H
#define ENTITY_H
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<random>
using namespace sf;
using namespace std;

class Entity:public Drawable {

protected:
	Vector2f speed;
	bool dead=false;
	float m; // to make things slower and stuff
	Sprite entity;
	Texture ent_txt;
	int i=0,j=0;
	Texture exp_txt;
	float angle=0;
	random_device rd;
	FloatRect rect;
	Clock c;
	bool onDestruction=false;
public:
	Entity();
	Entity& operator=(const Entity&)=delete;
	const Vector2f& getPosition() const;

	void move(float x,float y){
		entity.move(x,y);
		checkEdges();
	}
	void move(const Vector2f& v){
		move(v.x,v.y);
	}

	template<typename ...Ts>
	void setPosition(Ts&& ...args){
		entity.setPosition(std::forward<Ts>(args)...);
	}
	void rotate(float=M_PI/10);

	virtual void onDestroy();
	virtual void draw(RenderTarget& w,RenderStates) const override;

	void checkEdges() ;
	bool isDead() const noexcept;

	virtual void update() =0;

	// RVO ?
	auto getSize() const {
		return ent_txt.getSize();
	}
	bool isCollide(const Entity*) const ;

	const FloatRect& getRect() const {
		return rect;
	};
};


#endif