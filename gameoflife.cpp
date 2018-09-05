#include<iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<cstdlib>
using namespace std;
using namespace sf;
#define size 4

int main() {

	RenderWindow w(VideoMode(600,600)," it's alive !!");
	int cells[600/size][600/size];
	int states[600/size][600/size];
	Color COLOR;
	bool space=true;
	
	for(int i=0;i<600/size;i++)
		for(int j=0;j<600/size;j++)
		{
			cells[i][j]=0;
			states[i][j]=rand()%2;
		}
	
	// test differenet patterns
	auto set=[&](int x,int y,string s){
		int p=0;
		for(auto c: s)
		{
			states[x][y+p]= c=='#'? 1 : 0;
			++p;
		}
	};	

	while(w.isOpen())
	{
		if (space == true){
			
		w.clear(Color::Black);
		for(int i=1;i<(600/size)-2;++i)
			for(int j=1;j<(600/size)-2;j++)
			{
				cells[i][j]=states[i][j];
				int neighbours=cells[i+1][j]+cells[i-1][j]
								+cells[i][j-1]+cells[i][j+1]
								+cells[i-1][j-1]+cells[i+1][j+1]
								+cells[i-1][j+1]+cells[i+1][j-1];
				if(cells[i][j]) // if alive
					{
					states[i][j]= neighbours==3 || neighbours==2;
					// cell become dead if neighbours are less than 2 or greater than 3
					COLOR=Color::White;
					}
				else // if dead 
					{
					states[i][j]= neighbours==3;
					// become a live if numb neighbours is exactly 3 
					COLOR=Color::Black;
					}
				cout<<states[i][j]<<endl;	
				// drawing  stuff
				RectangleShape rect(Vector2f(size,size));
				rect.setPosition(Vector2f(i+size*i,j+size*j));
				rect.setFillColor(COLOR);
				w.draw(rect);
			}
		// update states of cells
		}	
		Event event;
		while(w.pollEvent(event))
		{
			if(event.type==Event::Closed)
				w.close();
			else if(event.type==Event::KeyPressed && event.key.code == Keyboard::Key::Space)
				space=false;
			else if(event.type==Event::KeyReleased && event.key.code == Keyboard::Key::Space)
				space=true;
			// space to pause			
		}		
	w.display();	

	}	
}