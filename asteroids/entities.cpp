#include<SFML/Graphics.hpp>
#include"config.h"
#include"entity.h"
#include"entities.h"
#include"game.h"
#include<iostream>


Enemy::Enemy():Entity(){
	entity.setPosition(rd()%Config::W_WIDTH,20);
}	


// metero stuff

Metero::Metero(float _m):
	Enemy(),metero(entity),metero_txt(ent_txt){
	type="metero";	
	m=_m; // set mass 
	string file_name="resources/images/";
	if(m < 5)
		file_name+="small.png";
	else if(m >=5 && m <= 10)
		file_name+="meduim.png";
	
	else 
		file_name+="big.png";
	metero_txt.loadFromFile(file_name);
	metero.setTexture(metero_txt);
	metero.setOrigin(metero.getTextureRect().width/2,metero.getTextureRect().height/2);

	// just testing
	speed=Vector2f(rd()%2,rd()%4)/m;
}


void Metero::fall(){
	// make a simple fall let it be better later
	move(speed);
	rotate();
}

void Metero::update(){
	fall();
	rect=entity.getGlobalBounds();
	if(onDestruction)
		onDestroy();
	//  and maybe some other stuff
}


void Metero::applyForce(const Vector2f& f){
	speed.x+=f.x;
	speed.y+=f.y;
}

void Metero::onCollide(Entity* e){
	if( not getRect().intersects(e->getRect()))
		return ;

	if(e->getType() != "metero"){
		this->onDestroy();
		e->onDestroy();
	}

	else{
		static_cast<Metero*>(e)->applyForce(0.01f*this->getForce());
		this->applyForce(static_cast<Metero*>(e)->getForce());
	}
	
}

// ship 

Ship::Ship():Entity(),ship(entity),ship_txt(ent_txt){
	type="ship";	
	ship_txt.loadFromFile("resources/images/ship.png");
	ship.setTexture(ship_txt);
	ship.setPosition(Config::W_WIDTH/2,Config::W_HEIGHT-100);
}

void Ship::jumpToHyperSpace(){
	// play effect ?
	setPosition(rd()%(Config::W_WIDTH-getSize().x)
		,rd()%(Config::W_HEIGHT-getSize().y));
}

// firing
void Ship::fire(Game& game)  {
	if(!onDestruction && c.getElapsedTime().asSeconds() >= BULLET_DELAY ){
		game.addEntity(new Bullet(this));
		c.restart();
	}
}

void Ship::update(){

	if(onDestruction){
		onDestroy();
		return;
	}

	if(Keyboard::isKeyPressed(Keyboard::Key::Up)){
		if(Keyboard::isKeyPressed(Keyboard::Key::Left))
			angle=-1/M_PI;

		else if(Keyboard::isKeyPressed(Keyboard::Key::Right))
			angle=1/M_PI;
				
		else {
			if(static_cast<int>(entity.getRotation())==0)
				angle=0.f;
		}
		move(sin(angle),-cos(angle));
		rotate(angle);
	}

	else if(Keyboard::isKeyPressed(Keyboard::Key::Space))
		jumpToHyperSpace();
	else if(Keyboard::isKeyPressed(Keyboard::Key::Left))
		move(-1,0);
	else if(Keyboard::isKeyPressed(Keyboard::Key::Right))
		move(1,0);
	if(Keyboard::isKeyPressed(Keyboard::Key::A))
		fire(game);

	rect=entity.getGlobalBounds();	
	// 
}

// shot

Bullet::Bullet(const Entity* e):bullet(entity),bullet_txt(ent_txt){
	type="bullet";	
	bullet_txt.loadFromFile("resources/images/bullet.png");
	bullet.setTexture(bullet_txt);
	angle=e->getAngle();
	bullet.setTextureRect(IntRect(0,0,Config::BULLET_SIZE,Config::BULLET_SIZE));
	force=SHOT_MAG*Vector2f(sin(angle),-cos(angle));
	// change this later to start from middle exactly
	bullet.setPosition(e->getPosition());
	i=2;
	bullet.rotate(100*angle);
}

void Bullet::update(){
	bullet.move(force);	
	--i;
	if(i<0)
		i=2;
	bullet.setTextureRect(IntRect(0,i*Config::BULLET_SIZE,
		Config::BULLET_SIZE,Config::BULLET_SIZE));	
	if(getPosition().x >= Config::W_WIDTH || getPosition().x <= 0 
		|| getPosition().y <= 0 || getPosition().y >= Config::W_HEIGHT)
		dead=true;
	// if dead it won't be drawn again
}


