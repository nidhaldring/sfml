#include<cassert>
#include<iostream>
#include"mario.h"
#include "music.h"
#define HEIGHT_LIMIT 150
#define GROUND 184
using namespace std;


mario::mario() : pos(0,184), speed(3.8,0),current_speed(speed),jump_speed(4,-4) {
	assert(img.loadFromFile("resources/images/mario.png"));
	NORMAL_SMALL_MARIO_RIGHT[0]= new Sprite(img,IntRect(178,32,12,16));
	// standing 
	NORMAL_SMALL_MARIO_RIGHT[1]= new Sprite(img,IntRect(80, 32, 15, 16));
	// moving 1
	NORMAL_SMALL_MARIO_RIGHT[2]= new Sprite(img,IntRect(96, 32, 16, 16));
	// moving 2
	NORMAL_SMALL_MARIO_RIGHT[3]= new Sprite(img,IntRect(112, 32, 16, 16));
	// moving 3
	NORMAL_SMALL_MARIO_RIGHT[4]= new Sprite(img,IntRect(160, 32, 15, 16));
	// dead mario
	NORMAL_SMALL_MARIO_RIGHT[5]= new Sprite(img,IntRect(144, 32, 16, 16));
	// jumping
	stand();
	pos=current_mario->getPosition();
}

FloatRect mario::getRect() const {
	return current_mario->getLocalBounds();
}
bool mario::isJumping() const noexcept {
	return jumping;
}
bool mario::isDead() const noexcept {
	return death;
}
const Vector2f& mario::getPosition() const {
	return current_mario->getPosition();
} 

void mario::stand() noexcept {
	NORMAL_SMALL_MARIO_RIGHT[0]->setPosition(pos);
	current_mario=NORMAL_SMALL_MARIO_RIGHT[0];
}

void mario::updateScale() noexcept {
	for(int i=0;i<4;++i)
		NORMAL_SMALL_MARIO_RIGHT[i]->scale(-1,1);
}
void mario::move(sides&& s=sides::RIGHT) noexcept {
	if(isJumping())
		return ;
	// don't perform any movements unless mario is on ground 
	// else continue the damn jump 

	// neg if right else pos 
	if( s == sides::LEFT )
		current_speed= - speed;
	else 
		current_speed= speed;

	// if mario is moving update to the next frame 
	if(moving_frame == 4)
			moving_frame=0;	
				
	else{

		current_mario=NORMAL_SMALL_MARIO_RIGHT[moving_frame++];	
		if(current_side != s){
			updateScale();
			current_side=s;
		}
		
		// also update current_mario
		current_mario->setPosition(pos);
		current_mario->move(current_speed);
		// pos will always have the last pos of the last frame 
		pos=current_mario->getPosition();
	}	
}
bool  mario::update(Event& event){

	if(isJumping()){
		jump();
		return true;
		}

	bool updated=false;
	// moves mario acc to key
	switch(event.key.code){
		case Keyboard::Key::Right:
			move();
			updated=true;
			break;
		case Keyboard::Key::Left:
			move(sides::LEFT);
			break;	
		case Keyboard::Key::Down:
			if(state == states::NORMAL_SMALL)
				break;
		case Keyboard::Key::Up:
			jump();
			break;	
		default:
			stand();
	}

	return updated; // or nah ;

}

bool mario::jump() noexcept {

	playJumpingTheme();
	if(!jumping){
		current_mario=NORMAL_SMALL_MARIO_RIGHT[5];
		current_mario->setPosition(pos);
		jumping=true;
		current_jump_speed=jump_speed;

		if(last_jump_side != current_side)
			current_mario->scale(-1,1);

		if(current_side == sides::LEFT){
			current_jump_speed.x=-current_jump_speed.x;
			last_jump_side=sides::LEFT;
		}
		else
			last_jump_side=sides::RIGHT;

	}
	current_mario->move(current_jump_speed*dt);
	
	if(current_mario->getPosition().y <= HEIGHT_LIMIT){
		current_jump_speed.y=-jump_speed.y;
	}

	else if(current_mario->getPosition().y >= GROUND && isJumping()){
		pos=current_mario->getPosition();
		pos.y=GROUND;
		stand();
		jumping=false;
	}
	return current_side == sides::RIGHT;
}

void mario::draw(RenderTarget& target,RenderStates states=RenderStates::Default ) const {
	target.draw(*current_mario);
}	

void mario::outOfTime() noexcept {
	current_mario=NORMAL_SMALL_MARIO_RIGHT[4];
	current_mario->setPosition(pos);
	playOutOfTimeTheme();
	timeOut=true;
}

bool mario::isOutOfTime() const {
	return timeOut;
}

void mario::die() {
	playDeathTheme();
	current_mario=NORMAL_SMALL_MARIO_RIGHT[4];
	current_mario->setPosition(pos);
	death=true;
}

mario::~mario(){
	for(int i=0;i<5;++i)
		delete NORMAL_SMALL_MARIO_RIGHT[i];
}