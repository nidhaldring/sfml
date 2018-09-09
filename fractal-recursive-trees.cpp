#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include<cmath>
#include<thread>
#include<stack>
#define THICKNESS 3
#define END 8
#define RECURSIVE_END 5
#define SLEEP_TIME 0.5s
#define CHANGE 10
#define ANGLE 45
// DON'T CHANGE THIS ONE UNLESS U'R GONNA DO THE MATH AGAIN
using namespace std;
using namespace sf;


stack<RectangleShape*> cleanup;
// keep track of heap allocated varriables 

RectangleShape* line(const Vector2f& start,int length,int angle)
{

	RectangleShape* l= new RectangleShape(Vector2f(THICKNESS,length));
	l->setOrigin(0,length);
	l->setPosition(start);
	l->setFillColor(Color::White);
	l->rotate(angle);
	return l;
}

void trees(Vector2f pos,int angle,float length,RenderWindow& w, int i=0)
{
	if(i==RECURSIVE_END  || length ==10 || pos.y <= 0 ){
		RectangleShape* end= new RectangleShape(Vector2f(END,END));
		cleanup.push(end);
		end->setFillColor(Color::Red);
		end->setPosition(pos);
		w.draw(*end);
		w.display();
		return;
	}
	RectangleShape* branche=line(pos,length,angle);
	cleanup.push(branche);
	w.draw(*branche);
	w.display();
	this_thread::sleep_for(SLEEP_TIME);
	if(angle != 0)
		pos.y-=length/sqrt(2);
	else
		pos.y-=length;
	if(angle <0)
		pos.x+=-length/sqrt(2);
	else if(angle > 0)
		pos.x+=length/sqrt(2);
	trees(pos,ANGLE,length-CHANGE,w,++i);
	trees(pos,-ANGLE,length-CHANGE,w,i);
}
int main(){
	RenderWindow w(VideoMode(800,600),"trees");
	Vector2f initpos(300,450);
	while(w.isOpen())
	{
		Event event;
		while(w.pollEvent(event))
		{
			if(event.type == Event::Closed)
				w.close();
		}
		
		w.clear(Color::Black);
		trees(initpos,0,100,w);		
		
		//w.display();
		this_thread::sleep_for(1s);
		// do cleanup 
		RectangleShape* tmp;
		while(!cleanup.empty())
		{
			tmp=cleanup.top();
			cleanup.pop();
			delete tmp ;
			// free mem
		}
	}
}