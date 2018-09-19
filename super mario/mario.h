#ifndef MARIO_H
#define MARIO_H
#include<SFML/Graphics.hpp> 
using namespace sf;

class mario: public Drawable {

private:
	Texture img;
	Sprite* NORMAL_SMALL_MARIO_RIGHT[14];
	Sprite* current_mario;
	Vector2f pos;
	Vector2f speed,current_speed,jump_speed,current_jump_speed;
	short moving_frame=0;
	bool jumping=false;
	bool death=false;
	bool timeOut=false;
	// define mario state duh :v
	enum class states {
		NORMAL_SMALL,
		NORMAL_BIG
	};
	enum class sides {
		RIGHT=0,
		LEFT
	};
	sides current_side=sides::RIGHT;
	states state=states::NORMAL_SMALL;
	sides last_jump_side=sides::RIGHT;
	// speed
	Clock clock;
	float dt=0.1 ;
	//
	virtual void draw(RenderTarget& target,RenderStates states) const;
	void move(sides&&) noexcept ;
	void updateScale() noexcept;
public:
	mario();
	// if has been update it return true
	bool update(Event&);
	void stand() noexcept ;
	const Vector2f& getPosition() const ;
	bool isJumping() const noexcept ;
	bool isDead() const noexcept;
	void die();
	FloatRect getRect() const ;
	bool jump() noexcept;
	void outOfTime() noexcept;
	bool isOutOfTime() const;
	virtual ~mario();

};

#endif