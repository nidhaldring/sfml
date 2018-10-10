#include"game.h"
#include"config.h"
#include<iostream>
#include<thread>		
using namespace std;

Game::Game():w(VideoMode(800,600),"game"){
	ship=new Ship;
	entities.push_back(ship);
	loadBackground();
	createLvl();
	// change this later;
}

void Game::loadBackground(){
	back_txt1.loadFromFile("space.png");
	back_txt2.loadFromFile("space.png");
	background1.setTexture(back_txt1);
	background2.setTexture(back_txt2);
	background1.setOrigin(0,back_txt1.getSize().y);
	background2.setOrigin(0,back_txt2.getSize().y);
	background1.setPosition(0,0);
	background2.setPosition(0,back_txt2.getSize().y);
}

void Game::moveBackground(Sprite& b){
	if(b.getPosition().y < Config::W_HEIGHT+b.getTexture()->getSize().y)
		b.move(0,1);
	else
		b.setPosition(0,0);

}
void Game::updateBackground(){
	//MAYBE CHANGE IT TO FLOAT RECT LATER JUST TO DEC SPEED
	moveBackground(background1);
	moveBackground(background2);
}

void Game::renderBackground(){
	//  there's a small error but i'll pass  for now
	if(background1.getPosition().y >= 0)
		w.draw(background1);
	if(background2.getPosition().y >= 0)
		w.draw(background2);

}

void Game::renderEntities(){
	// remove dead objects
	entities.remove_if([](Entity* e){
		return e->isDead();
	});

	for(auto& entity:entities){
		w.draw(*entity);
		entity->update();
	}
}

void Game::createLvl(){
	// settings for ultra hard stuff ;
	int numBig=10,numMed=7,numSmall=5+lvl%2;
	// the smaller the harder the lvl ? 
	if( lvl <= Config::EASY_LVLS){
		numBig=4+lvl%2;
		numMed=3;
		numSmall=2;
	}
	else if( lvl <= Config::MED_LVLS){
		numBig=7+lvl%2;
		numMed=5+lvl%3;
		numSmall=5;
	}

	// create meteros duh :v 
	for(int i=0;i<numSmall;++i)
		entities.push_back(new Metero(rd()%Config::MAX_SMALL));
	

	for(int i=0;i<numMed;++i)
		entities.push_back(new Metero(rd()%Config::MAX_MED));

	for(int i=0;i<numBig;++i)
		entities.push_back(new Metero(rd()%Config::MAX_BIG));
	// increase current lvl 
	++lvl;
	// make music better ? change background ?
	// maybe ?
}

void Game::checkCollosion() const {
		for(auto& e:entities)
			if(ship->isCollide(e)){
				ship->onDestroy();			
			}
			if(ship->isDead())
				cout<<"dead"<<endl;
}

// main loop

void Game::run(){
	Event event;
	Metero m;
	Ship s;
	while(w.isOpen()){
		while(w.pollEvent(event)){
			if(event.type==Event::Closed)
				w.close();
		}
		w.clear();
		// render and update everything
		renderBackground();
		renderEntities();
		w.display();
		updateBackground();
		checkCollosion();
	}
}

Game::~Game(){
	for(auto& en:entities)
		delete en;
}