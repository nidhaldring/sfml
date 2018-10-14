#include"entity.h"
#include"config.h"

Entity::Entity(){
	rect=entity.getGlobalBounds();
	exp_txt.loadFromFile("resources/images/exp.png");
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

void Entity::rotate(float angle){
	this->angle=angle;
	entity.rotate(angle);
}

bool Entity::isDead() const noexcept{
	return dead;
}


void Entity::onDestroy(){
 
 	if(isDead())
 		return;
	if(!onDestruction){
		onDestruction=true;
		entity.setTexture(exp_txt);
	}
	
	entity.setTextureRect(IntRect(i*CONFIG_EXP,j*CONFIG_EXP,
		CONFIG_EXP,CONFIG_EXP));

	if(c.getElapsedTime().asSeconds() >= CONFIG_EXP_FPS){
		++i;
		c.restart();
		if( i*CONFIG_EXP >= Config::EXP_IMG_WIDTH){
			i=0;
			++j;
			}
		}
	
	if( j*CONFIG_EXP >= Config::EXP_IMG_HEIGHT)		
			dead=true;
}





