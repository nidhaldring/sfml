#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<thread>
#include<string>
#include"mario.h"
#include"music.h"
#include<iostream>
#define VIEW_SPEED 2.25,0
#define VIEW_SPEED_JUMP 0.3,0
// might need to do some math here
#define MAX_TIME 60
#define HURRY_TIME 50	
using namespace sf;
using namespace std;

// all holes in the game as a pair , (beg,end) 
int holes[]= {
	1102,1135,
	1376,1423,
	2447,2479
};

Vector2f operator/(const Vector2f& v, float x){

	return {v.x/x,v.y/2};
}

Vector2f operator-(const Vector2f& v,float x){
	return {v.x-x,v.y-x};
}

void setText(Text& txt,string&& s,const Font& f,int size=20,const Color& color=Color::Red){
	txt.setFont(f);
	txt.setCharacterSize(size);
	txt.setFillColor(color);
	txt.setString(s);
	txt.setStyle(sf::Text::Bold);
}
int main(){
	mario super_mario;
	RenderWindow w(VideoMode(800,224),"super mario");
	Texture map_img;
	View view=w.getDefaultView();
	bool key=false;
	//
	bool draw_timer=true;
	// used to make the effect of the clock ticking hurrying
	if(!map_img.loadFromFile("resources/images/map.png"))
	{
		return -1;
	}
	Sprite map(map_img);
	// loading stuff
	loadMusic();
	playMainTheme();
	// time and stuff
	Clock clock;
	int time;
	// main font 
	Font Font;
	assert(Font.loadFromFile("resources/fonts/Roboto-Black.ttf"));
	// timer stuff
	Text Timer;
	setText(Timer,std::to_string(MAX_TIME),Font);
	Vector2f TimerPos;
	// THIS POS IS USED FOR TIMER POS ,LOST POS AND TIME OUT POS !!!!!
	// OTHER TEXTS 
	// WHEN LOSING
	// WHEN TIME'S OUT
	Text LOST;
	setText(LOST," U'VE LOST ",Font);
	Text TIMEOUT;
	setText(TIMEOUT,"TIME OUT !",Font);
	Text* txt;
	// this pointer is used later for displaying one of both up;
	// 

	while(w.isOpen())
	{
		time=clock.getElapsedTime().asSeconds();
		Timer.setString("TIMER "+std::to_string(MAX_TIME-time));
		Event event;
		while(w.pollEvent(event))
		{
			if(event.type == Event::Closed)
				w.close();
			else if(event.type == Event::KeyPressed)
			{
				key=true;
				if(super_mario.update(event) && view.getCenter().x < 3392-400)
					view.move(VIEW_SPEED);
				// change viewonly if mario moved to the right 
			}
			else key=false;
		}

		if(!key){
			if(!super_mario.isJumping())
				super_mario.stand();
			else 
			{
				if(super_mario.jump()) 
					view.move(VIEW_SPEED_JUMP);
			}
			// continue the jump
		}
		
		//search for holes here 
		for(int i=0;i<6;i+=2)
		{
			if(super_mario.getPosition().x > holes[i] && super_mario.getPosition().x < holes[i+1])
			{
				if(!super_mario.isJumping()){
					super_mario.die();
					txt=&LOST;
					break;
				}
			}
		}
		// TIMER WILL FOLLOW THE VIEW;
		TimerPos=view.getCenter()+(view.getSize()/2)-108;
		TimerPos.y=0;
		Timer.setPosition(TimerPos);
		//
		w.setView(view);
		w.draw(map);
		w.draw(super_mario);
		if(draw_timer)
			w.draw(Timer);
		if(SpedUpMainThemePlaying())
			draw_timer= !draw_timer;
		w.display();

		// time measuring stuff 

		 if(time >= MAX_TIME){
			super_mario.outOfTime();
			txt=&TIMEOUT;
		}
		else if(time >= HURRY_TIME &&  !super_mario.isDead()){
			playSpedUpMainTheme();
		}
		// death animation 
		if(super_mario.isDead() || super_mario.isOutOfTime())
		{
			Vector2f pos=view.getCenter();
			//txt.setOrigin(0,)
			// maybe add this later
			txt->setPosition(pos);

			while(!DeathMusicEnded() || !OutOfTimeThemeEnded()){
				w.draw(map);
				w.draw(*txt);
				w.draw(super_mario);
				w.display();
			}
			w.close();
		}
		// ends here
	}
}