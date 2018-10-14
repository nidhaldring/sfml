#ifndef ENTITY_H
#define ENTITY_H
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<random>
#include<string>
using namespace sf;
using namespace std;

class Entity:public Drawable {

protected:
	Vector2f speed;
	float m,angle=0.f; //  m :to make things slower and stuff
	Sprite entity;
	Texture ent_txt,exp_txt;
	short i=0,j=0;
	random_device rd;
	FloatRect rect;
	Clock c;
	bool dead=false,onDestruction=false;
	string type;
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

	const FloatRect& getRect() const {
		return rect;
	};
	float getAngle() const {
		return angle;
	}

	const string& getType() const {
		return type;
	}


};


#endif