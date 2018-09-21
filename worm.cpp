#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<vector>
#include<iostream>
#include<cstdlib>
using namespace std;
using namespace sf;
#define SPECIAL_FOOD_TIME 10


class Food: public Drawable {
	bool special;
	Vector2f pos;
	RectangleShape food;
public:
	Food():food(Vector2f(10,10)),pos(((rand()%80)*10),((rand()%60)*10)) {
			food.setFillColor(Color::Red);
			food.setPosition(pos);
	}
	void update() {
		pos.x=(rand()%80)*10;
		pos.y=(rand()%60)*10;
		food.setPosition(pos);
		special=(rand()%2 == 0);
		if(isSpecial())
			food.setFillColor(Color::Blue);
		else
			food.setFillColor(Color::Red);
	}
	virtual void draw(RenderTarget& target,RenderStates state=RenderStates::Default) const {
		target.draw(food);
	}
	
	bool isSpecial() const {
		return special;
	}
	const Vector2f& getPosition() const {
		return pos;
	}
};

class Worm :public Drawable {
	int l=50;
	mutable vector<RectangleShape*> worm;
	short score=0;
	Vector2f headPos;
	Keyboard::Key dir=Keyboard::Key::Up;
	float dt;
	Clock clock;
	bool added=false;
public:
	Worm(){
		for(int i=0;i<l;++i){
			worm.push_back(new RectangleShape(Vector2f(10,10)));
			worm[i]->setPosition(Vector2f(300,300+i*10));
		}
		headPos=worm[0]->getPosition();
		clock.restart();
	}

	virtual void draw(RenderTarget& target,RenderStates states=RenderStates::Default) const{
		for(auto i:worm)
			target.draw(*i);
	}
	bool moveHead(Keyboard::Key& key){
		switch(key){
			case Keyboard::Key::Up:
				if(dir == Keyboard::Key::Down)
					return false;
				if(worm[0]->getPosition().y <= 0)
					worm[0]->setPosition(worm[0]->getPosition().x,600);
				worm[0]->move(0,-10);
				break;
			case Keyboard::Key::Down:
				if(dir == Keyboard::Key::Up)
					return false;
				if(worm[0]->getPosition().y >= 600)
					worm[0]->setPosition(worm[0]->getPosition().x,0);
				worm[0]->move(0,10);
				break;
			case Keyboard::Key::Left:
				if(dir == Keyboard::Key::Right)
					return false;
				if(worm[0]->getPosition().x <= 0)
					worm[0]->setPosition(800,worm[0]->getPosition().y);
				worm[0]->move(-10,0);
				break;
			case Keyboard::Key::Right:
				if(dir == Keyboard::Key::Left)
					return false;
				if(worm[0]->getPosition().x >= 800)
					worm[0]->setPosition(0,worm[0]->getPosition().y);
				worm[0]->move(10,0);
				break;
			default:
			return false;				
		}
		return true;
	}

	void apply(){
		if(added){
			l++;
			added=false;
		}
		for(int i=l-1;i!=0;--i){
			worm[i]->setPosition(worm[i-1]->getPosition());
		}
	}
	bool update(Keyboard::Key& key){
		if(moveHead(key)){
			apply();
			headPos=worm[0]->getPosition();
			dir=key;
			return true;
		}
		return false;
	}
	
	void keepMoving(){
		dt=clock.getElapsedTime().asSeconds();
		if(dt >= 0.048){
			update(dir);
			clock.restart();
			dt=0;
		}
	}

	void eat(Food& food){
		if(food.isSpecial())
			score+=10;
		else
			score+=3;
		worm.push_back(new RectangleShape(Vector2f(10,10)));
		added=true;
		food.update();
	}

	short getScore() const {
		return score;
	}

	bool operator==(Food& f){
		return worm[0]->getPosition()==f.getPosition();
	}
	bool isDead(){
		for(int i=3;i<l;++i)
				if(worm[0]->getPosition() == worm[i]->getPosition())
					return true;
		return false;		
		}
	virtual ~Worm(){
		for(auto i:worm)
			delete i;
	}	
};

int main() {

	RenderWindow w(VideoMode(800,600),"worm");
	Worm worm;
	Food food;
	bool updated;
	Clock clock;
	float dt=0;
	bool first=true;
	cout<<" collect  50 points to win :) "<<endl;
	cout<<"your score is:"<<endl;
	while(w.isOpen()){
		updated=false;
		Event event;
		while(w.pollEvent(event)){
			if(event.type == Event::KeyPressed)
				updated=worm.update(event.key.code);
			else if(event.type == Event::Closed)
				w.close();
			}
		if(!updated)
			worm.keepMoving();
		if(worm == food){
			worm.eat(food);
			first=true;
		}

		w.clear();
		w.draw(worm);
		if(food.isSpecial()){
			if(first){
				first=false;
				clock.restart();
			}
			dt=clock.getElapsedTime().asSeconds();
			if(dt >= SPECIAL_FOOD_TIME ){
				food.update();
				first=true;
			}
		}
		
		w.draw(food);
		w.display();
		cout<<worm.getScore()<<'\r'<<flush;
		if(worm.isDead()){
			cout<<'\n'<<" u've lost "<<endl;
			w.close();
		}

		if(worm.getScore() >= 50){
			cout<<'\n'<<" u've win"<<endl;
			w.close();
		}
	}
}