#include"entities.h"
#include<iostream>
#include"config.h"
#include<cstdlib>



Enemy::Enemy():Entity(){
	entity.setPosition(rd()%Config::W_WIDTH,20);
}	


// metero stuff

Metero::Metero(float _m):Enemy(),metero(entity),metero_txt(ent_txt){
	m=_m; // set mass 
	std::string file_name;
	if(m < 5)
		file_name="small.png";
	else if(m >=5 && m <= 10)
		file_name="meduim.png";
	
	else 
		file_name="big.png";
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
	speed.x=rd()%2;
	rect=entity.getGlobalBounds();
	//  and maybe some other stuff
}


// ship 

Ship::Ship():Entity(),ship(entity),ship_txt(ent_txt){
	ship_txt.loadFromFile("ship.png");
	ship.setTexture(ship_txt);
	ship.setPosition(Config::W_WIDTH/2,Config::W_HEIGHT-100);
}

void Ship::jumpToHyperSpace(){
	// play effect ?
	setPosition(rd()%(Config::W_WIDTH-getSize().x)
		,rd()%(Config::W_HEIGHT-getSize().y));


	//rotate(static_cast<float>(rd()%360));
	// maybe check it later ?
}

void Ship::update(){
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
	rect=entity.getGlobalBounds();	
}

