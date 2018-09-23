#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
using namespace sf;

const Vector2f center(400,300);
constexpr float angle=4.f;

float distance(const Vector2f& a, const Vector2f& b){
	return sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2));
}


class Ball:public Drawable {

	CircleShape circle;
public:
	bool intacted=false;
	Color color;
	bool dead=false;
	Ball():circle(10,50){
		
		circle.setFillColor(Color(0,0,255));
		circle.setPosition(Vector2f(400,550));
	}
	void move() noexcept {
		circle.move(0,-3.5);
	}
	virtual void draw(RenderTarget& w,RenderStates=RenderStates::Default) const {
		w.draw(circle);
	}

	const Vector2f& getPosition() const {
		return circle.getPosition();
	}
	float getR() const {
		return circle.getRadius();
	}

	void reset() {
		circle.setPosition(Vector2f(400,550));
		intacted=false;
	}
	
};

class arc:public Drawable {
	int l=10;
	CircleShape* p;
	Transform orbit;
	Color color;
	float y;
	bool done=false;
public:
	arc(Color color=Color(100,100,100),int y=center.y,Vector2f center=Vector2f(400,300))
	:color(color),y(y) {

		p=new CircleShape[l];
		for(int i=0;i<l;++i){
			p[i].setFillColor(color);
			p[i].setPointCount(500);
			p[i].setPosition(300-i*angle+0.1,y);
			p[i].setRadius(angle);
		}
		orbit.rotate(angle,center);

	}

	void update(){
		for(int i=l-1;i!=0;--i)
			p[i].setPosition(p[i-1].getPosition());
		p[0].setPosition(orbit.transformPoint(p[0].getPosition()));
	}

	virtual void draw(RenderTarget& w,RenderStates=RenderStates::Default) const {
		for(int i=0;i<l;++i)
			w.draw(p[i]);
	}

	bool isDone() {
		if(!done && p[l-1].getPosition() == Vector2f(300+0.1,y))
			done=true;
		return done;
	}

	void intact(Ball& b){
		for(int i=0;i<l;++i)
			if(distance(p[i].getPosition(), b.getPosition()) <= b.getR() ){	
				if(!b.intacted){
					b.intacted=true;
					b.color=p[i].getFillColor();
				}

				else {
					// if has already intacted
					if(p[i].getFillColor() != b.color){
						b.dead=true;	
				
					}
					break;
				}
			}
			
	}
};




int main() {
	RenderWindow w(VideoMode(800,600),"color switch ?");
	CircleShape p(8,500);
	// point on center stuff 
	p.setFillColor(Color(255,0,0));
	p.setPosition(Vector2f(400,300));
	//
	Ball ball;
	//
	Event event;
	Clock clock;
	float dt;
	float limit=0.05;
	bool start=false;
	vector<arc*> list;
	Color c(0,0,0) ;

	for(int i=0;i<10;++i){
		if(i%2){
			c.r=255;
			c.b=0;
		}
		else {
			c.r=0;
			c.b=255;
		}

		list.push_back(new arc(c));
	
	}
	int i=0;
	while(w.isOpen()){
		while(w.pollEvent(event)){
			if(event.type == Event::Closed)
				w.close();
			else if(event.type ==Event::KeyPressed && list[9]->isDone()){
				if(event.key.code == Keyboard::Key::Space)
					start=true;
			}
		}

		dt=clock.getElapsedTime().asSeconds();
		w.clear();
		w.draw(p);

		for(int j=0;j<=i;j++)
			w.draw(*list[j]);

		w.draw(ball);	
		w.display();
		if(dt >= limit) { 
			clock.restart();
			if(list[i]->isDone())
				if(i < 9)
					++i;

			for(int j=0;j<=i;++j)
				list[j]->update();	

			}
	
		if(start) 
				ball.move();
		if (ball.getPosition().y <= 0){
			start=false;
			ball.reset();
		}
		// check for intact
		for(auto i:list){
			i->intact(ball);
			if(ball.dead){
				cout<<" you dead"<<endl;
				w.close();
				break;
			}
		}
	}
}