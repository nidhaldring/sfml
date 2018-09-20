#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<queue>
#include<iostream>
#include<cstdlib>
using namespace std;
using namespace sf;
#define speed 10



class Food: public Drawable {
	bool special=rand()%10 ==9;
	Vector2f pos;
	RectangleShape food;
public:
	Food():food(Vector2f(10,10)),pos(((rand()%80)*10),((rand()%60)*10)) {
			food.setFillColor(Color::Red);
			food.setPosition(pos);
			cout<<pos.x<<" "<<pos.y<<endl;
	}
	void update() {
		pos.x=(rand()%80)*10;
		pos.y=(rand()%60)*10;
		food.setPosition(pos);
		special=(rand()%10 == 9);
		if(isSpecial())
			food.setFillColor(Color::Blue);
		else
			food.setFillColor(Color::Red);
		cout<<pos.x<<" "<<pos.y<<endl;
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
	int l=8;
	// len of worm
	int score=0;
	mutable queue<RectangleShape*> worm;
	RectangleShape* head;
	Vector2f headPos,tailPos;
	Keyboard::Key dir;
	Clock clock;
	float dt;
public:
	Worm(){
		head=new RectangleShape(Vector2f(10,10));
		head->setPosition(300,300);
		headPos=head->getPosition();
		worm.push(head);
		RectangleShape* tmp;
		for(int i=1;i<l;++i){
			tmp=new RectangleShape(Vector2f(10,10));
			tmp->setPosition(300,300+i*10);
			worm.push(tmp);
		}
		tailPos=tmp->getPosition();
		dir=Keyboard::Key::Up;
		clock.restart();
	}

	void apply() {
		//apply to rest
		// the head should be tail now
		Vector2f pos,tmp;
		pos=headPos;
		for(int i=0;i<l-1;++i){
			tmp=worm.front()->getPosition();
			worm.front()->setPosition(pos);
			worm.push(worm.front());
			worm.pop();
			pos=tmp;
		}
		tailPos=tmp;
		// apply to all :v 
	}
	//
	bool moveHead(Keyboard::Key& key){
		switch(key){
			case Keyboard::Key::Up:
				if(dir == Keyboard::Key::Down)
					return false;
				head->move(0,-speed);
				if(head->getPosition().y<=0)
					head->setPosition(Vector2f(headPos.x,600));
				break;

			case Keyboard::Key::Down:
				if(dir == Keyboard::Key::Up)
					return false;
				head->move(0,speed);
				if(head->getPosition().y>=600)
					head->setPosition(Vector2f(headPos.x,0));
				break;
			case Keyboard::Key::Right:
				if(dir == Keyboard::Key::Left)
					return false;
				head->move(speed,0);
				if(head->getPosition().x >= 800)
					head->setPosition(Vector2f(0,headPos.y));
				break;
			case Keyboard::Key::Left:
			if(dir == Keyboard::Key::Right)
					return false;
				head->move(-speed,0);
			if(head->getPosition().x <= 0)
				head->setPosition(Vector2f(800,headPos.y));
				break;	
			default:
				return false;	
		}
		return true;
	}

	bool update(Keyboard::Key& key){
		//dir=key;
		if(moveHead(key)){
			dir=key;
			worm.push(worm.front());
			worm.pop();
			apply();
			headPos=head->getPosition();
			return true;
		}
		return false;
		
	}
	// keep on moving 
	void move(){
		dt=clock.getElapsedTime().asSeconds();
		if(dt>=0.05){
			clock.restart();
		}
		else return;
		moveHead(dir);
		worm.push(worm.front());
		worm.pop();
		apply();
		cout<<"here"<<endl;
		headPos=head->getPosition();
	}

	
	virtual void draw(RenderTarget& target,RenderStates states=RenderStates::Default) const{
		RectangleShape* tmp;
		for(int i=0;i<l;++i){
			tmp=worm.front();
			target.draw(*tmp);
			worm.push(tmp);
			worm.pop();
		}
	}

	const Vector2f& getPosition() const {
		return head->getPosition();
	}

	void eat(Food& food){
		if(food.isSpecial())
			score+=10;
		else
			score+=3;
		l+=1;
		RectangleShape* n= new RectangleShape(Vector2f(10,10));
		n->setPosition(tailPos);
		worm.push(n);
		food.update();

	}

	int getScore() const {
		return score;
	}

};





int main() {

	RenderWindow w(VideoMode(800,600),"worm");
	Worm worm;
	Food food;
	bool updated;
	while(w.isOpen()){
		updated=false;
		Event event;
		while(w.pollEvent(event)){
			if(event.type == Event::KeyPressed)
				updated=worm.update(event.key.code);
			}
		if(!updated)
			worm.move();
		if(worm.getPosition()==food.getPosition())
			worm.eat(food);
		w.clear();
		w.draw(worm);
		w.draw(food);
		w.display();
		cout<<worm.getScore()<<endl;
		if(worm.getScore() >= 40){
			cout<<" u've win"<<endl;
			w.close();
		}
	}
}