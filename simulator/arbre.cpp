#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<cmath>
#include<iostream>
#include<list>
#include<unordered_map>
using namespace sf;
using namespace std;
#define WIDTH 800
#define HEIGHT 600
#define ROOT_VECTOR Vector2f(WIDTH/2,20)
#define R 20
#define LINE Vector2f(1,100)
#define FCT 1/sqrt(2)
#define HEIGHT_DIFF LINE.y
#define SLELECTION_COLOR Color(0,0,255) // Blue
#define DEFAULT_COLOR 	Color(255,255,255) // white
#define NOTHING 0



class node:public Drawable{
private:
	CircleShape __node;
	Color color;
	const node *parent;
	mutable node *left,*right;
	RectangleShape l1,l2;
	Text data_txt;
	Font font;
public:

	node(const node&)=delete;
	node& operator=(const node&)=delete;
	node():color(255,255,255),left(nullptr),
	right(nullptr),__node(R,200),l1(LINE),l2(LINE)
	{
		__node.setFillColor(color);
		__node.setOrigin(R,R);
		__node.setPosition(ROOT_VECTOR);
		l1.rotate(45);
		l2.rotate(-45);
		l1.setPosition(__node.getPosition().x,__node.getPosition().y);
		l2.setPosition(__node.getPosition().x,__node.getPosition().y);
		l1.setFillColor(Color(255,0,0));
		l2.setFillColor(Color(0,255,0));
		font.loadFromFile("1.ttf");
		data_txt.setCharacterSize(30);
		data_txt.setFont(font);
		data_txt.setString("0");
		data_txt.setFillColor(Color(0,0,0));
		data_txt.setPosition(__node.getPosition().x-10,__node.getPosition().y-15);
		// init pointers
		left=right=nullptr;
		parent=nullptr;
	}

	void draw( RenderTarget& w, RenderStates=RenderStates::Default) const override{
		// draw node and lines
		if(left!=nullptr)
			w.draw(l1);
		if(right!=nullptr)
			w.draw(l2);
		w.draw(__node);
		w.draw(data_txt);
	}
	const Vector2f& getPosition() const {
		return __node.getPosition();
	}

	// setters 
	void setParent(const node* p){
		parent=p;
	}

	void setPosition(float x,float y){
		__node.setPosition(x,y);
		data_txt.setPosition(x-10,y-15);
		l1.setPosition(__node.getPosition().x,__node.getPosition().y);
		l2.setPosition(__node.getPosition().x,__node.getPosition().y);
	}
	void setLeftNode(node* x) const {
		if(x){
			x->setPosition(getPosition().x-HEIGHT_DIFF/sqrt(2),
				getPosition().y+HEIGHT_DIFF/sqrt(2));
			x->setParent(this);
		}

		left=x;
		
	}
	void setRightNode(node* y) const {
		if(y){
			y->setPosition(getPosition().x+HEIGHT_DIFF/sqrt(2),
				getPosition().y+HEIGHT_DIFF/sqrt(2));
			y->setParent(this);
		}

		right=y;
		
	}
	void setData(char s){
		data_txt.setString(s);
	}
	void setColor(Color&& c){
		color=std::move(c);
		// might not need color !!! 
		// might be redundant 
		__node.setFillColor(color);
	}
	//state 
	node* getRight() const {return right;}
	node* getLeft() const {return left;}

	bool hasRight() const { return right!=nullptr; }
	bool hasLeft() const { return left!=nullptr; }
	bool hasChildren(){
		return hasRight() && hasLeft();
	}

	float getR() const {
		return __node.getRadius();
	}

	const node* getParent() {
		return parent;
	}

	bool collide(Vector2i& Mpos) const {
		return sqrt(pow(Mpos.x-this->getPosition().x,2)+
			pow(Mpos.y-this->getPosition().y,2)) <= this->getR();

	}
};
// node end here ;


class WINDOW {
	// CUZ SF HAS Window
private:
	RenderWindow w;
	list<node*> nodes;
	node* selected_node,*prev_selected_node; // might need prev 
	// text and buttons
	Font font;
	list<Text*> texts ; 
	//mouse and stuff
	bool mouse_pressed=true;
	Vector2i Mpos;

private:
	void selectText (Text* txt) const {
		txt->setFillColor(SLELECTION_COLOR);
	}

	void unselectText(Text* txt) const {
		txt->setFillColor(Color(255,255,255));
	}
	//

	void add_node(char data){
		if(selected_node==nullptr){
			cout<<" root has been created !"<<endl;
			selected_node=new node;
			selected_node->setData(std::move(data));
			nodes.push_back(selected_node);
			selected_node->setColor(SLELECTION_COLOR);
		}

		else if(selected_node->hasChildren())
			cerr<<" this node already has childreen"<<endl;

		else{
			// by default all node gets added to the right
			// change by swap
			node* n=new node;
			n->setData(std::move(data));
			if(!selected_node->hasRight())
				selected_node->setRightNode(n);
			else
				selected_node->setLeftNode(n);
			nodes.push_back(n);
		}
	}	

	void delete_node(){
		if(selected_node==nullptr)
			cerr<<" make sure u select a node before !"<<endl;
		else if(selected_node->hasChildren())
			cerr<<" the selected node has childreen !";
		else{
			const node *right,*left,*parent;
			parent=selected_node->getParent();
			if(parent){
				left=parent->getLeft();
				right=parent->getRight();
				if(selected_node == left )
					parent->setLeftNode(nullptr);
				else
					parent->setRightNode(nullptr);
			}
			nodes.remove(selected_node);	
		}
	}

	void swap_nodes(){

	}

	void mapTxtToFct(const Text* txt) {
		if(txt->getString()=="add_node"){
			cout<<" write data to node: "<<flush;
			this->add_node(cin.get());
			cin.get(); //
		}

		 else if(txt->getString()=="delete_node") {
		 	this->delete_node();
		 	selected_node=nullptr;
		 }

	}

	bool collide(Vector2i& Mpos,Text* txt) const {
		return
		Mpos.y >= txt->getPosition().y 
		&& Mpos.y <= txt->getPosition().y+txt->getCharacterSize()
		&& Mpos.x >= txt->getPosition().x
		&& Mpos.x < 
		txt->getPosition().x+txt->getCharacterSize()*(txt->getString().getSize()/2+1);
	}


	void checkMouseTextCollosion(Vector2i& Mpos)  {
		// check for collosion
		for(Text* txt:texts){
			if(collide(Mpos,txt)){
					mapTxtToFct(txt);
					break;
				}
			// magic 	
		}
	}

public:
	WINDOW(const WINDOW&)=delete;
	WINDOW& operator=(const WINDOW&)=delete;
	WINDOW():selected_node(nullptr),w(VideoMode(WIDTH,HEIGHT),"arbre"){
		//
		font.loadFromFile("1.ttf");
		texts.push_back( new Text("add_node",font));
		texts.push_back(new Text("delete_node",font));
		int i=50;
		auto init_txt=[&i](Text* txt){
			txt->setStyle(sf::Text::Bold | sf::Text::Italic | Text::Underlined);
			txt->setPosition(5,i);
			i+=50;
		};

		for(auto txt:texts)
			init_txt(txt);
		// txt prep done 

		selected_node=nullptr;
		prev_selected_node=nullptr;
	}

	void run(){
		// here we go 
		Event event;
		while(w.isOpen()){
			// 
			Mpos=Mouse::getPosition(w);

			// handle events 
			while(w.pollEvent(event)){
				if(event.type==Event::Closed)
					w.close();
				else if(event.type==Event::MouseButtonPressed){
					if(mouse_pressed){
						checkMouseTextCollosion(Mpos);
						// check if mouse selected a node and change to it
						for(node* n:nodes){
							if(n->collide(Mpos)){
								selected_node->setColor(DEFAULT_COLOR); // CHANGE FOCUSS
								selected_node=n;
								selected_node->setColor(SLELECTION_COLOR);
								cout<<"node has been selected"<<endl;
								break;
							}
						}
						mouse_pressed=false;
					}
				}
				else if(event.type==Event::MouseButtonReleased)
					mouse_pressed=true;
			}
			// render entitites 
			w.clear();

			for(node* n:nodes)
				w.draw(*n);

			for(Text* txt:texts){
				if(collide(Mpos,txt))
					selectText(txt);
				else
					unselectText(txt);
				w.draw(*txt);
			}
			
			w.display();
	}

}

	~WINDOW(){
		for(auto n:nodes)
			delete n;
		for(auto txt:texts)
			delete txt;
	}
};

//ends here


int main(){
	
	WINDOW w;
	w.run();
}