#include<SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include<set>
#include<time.h>
#include <string>

#define MAX_NUMBER_OF_ITEMS 3

using namespace sf;
using namespace std;

class Menu                            // CLASS TO DISPLAY VARIOUS OPTIONS FOR PLAYER 
{
public:
	Menu(float width, float height,string opt1,string opt2,string opt3);
	~Menu();

	void draw(sf::RenderWindow &window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	Font font;
	Text menu[MAX_NUMBER_OF_ITEMS];

};

Menu::Menu(float width, float height,string opt1,string opt2,string opt3) //to display the menu with options passed as arguments
{
	font.loadFromFile("font/Pirata_One/PirataOne-Regular.ttf");

	menu[0].setFont(font);
	menu[0].setColor(Color::Red);
	menu[0].setString(opt1);
	menu[0].setPosition(Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 1));

	menu[1].setFont(font);
	menu[1].setColor(Color::White);
	menu[1].setString(opt2);
	menu[1].setPosition(Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 2));

	menu[2].setFont(font);
	menu[2].setColor(Color::White);
	menu[2].setString(opt3);
	menu[2].setPosition(Vector2f(width / 2, height / (MAX_NUMBER_OF_ITEMS + 1) * 3));

	selectedItemIndex = 0;
}


Menu::~Menu()
{
}

void Menu::draw(sf::RenderWindow &window)     //draw the menu on window
{
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++)
	{
		window.draw(menu[i]);
	}
}

void Menu::MoveUp()                          //used for navigating between options
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setColor(Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setColor(Color::Red);
	}
}

void Menu::MoveDown()                       //used for navigating between options
{
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS)
	{
		menu[selectedItemIndex].setColor(Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setColor(Color::Red);
	}
}
