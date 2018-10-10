#include"entity.h"
#include"config.h"

Entity::Entity(){
	rect=entity.getGlobalBounds();
	exp_txt.loadFromFile("exp.png");
}

const Vector2f& Entity::getPosition() const {
	return entity.getPosition();
}


void Entity::draw(RenderTarget& w,RenderStates=RenderStates::Default) const{
	w.draw(entity);
}

void Entity::checkEdges(){
	if(getPosition().x <= 0)
		setPosition(Config::W_WIDTH,getPosition().y);
	
	else if(getPosition().x >= Config::W_WIDTH)
		setPosition(0,getPosition().y);

	else if(getPosition().y <= 0)
		setPosition(getPosition().x,Config::W_HEIGHT);
	
	else if(getPosition().y >= Config::W_HEIGHT)
		setPosition(getPosition().x,0);
	

}

void Entity::rotate(float _angle){
	angle=_angle;
	entity.rotate(angle);
}

bool Entity::isDead() const noexcept{
	return dead;
}


void Entity::onDestroy(){
 
	if(!onDestruction){
		entity.setTexture(exp_txt);
		onDestruction=true;
	}
	// SMTHNG MAKING THIS VERY SLOW 
	entity.setTextureRect(IntRect(i,j,CONFIG_EXP,CONFIG_EXP));
	if(c.getElapsedTime().asSeconds() >= CONFIG_EXP_FPS){
		cout<<c.getElapsedTime().asSeconds()<<endl;
		j+=CONFIG_EXP;
		c.restart();
		if( j >= Config::EXP_IMG_WIDTH){
			j=0;
			i+=CONFIG_EXP;
		}
		if( i >= Config::EXP_IMG_HEIGHT)
				dead=true;	
	}
}

bool Entity::isCollide(const Entity* entity) const {
	// not enough
	// color checking next
	return  dynamic_cast<decltype(this)>(entity)!= this 
		&& (entity->getRect().intersects(getRect()) 
		|| getRect().intersects(entity->getRect()));
}



