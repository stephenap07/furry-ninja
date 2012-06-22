#pragma once

#include <SFML/Graphics.hpp>

//this is how to make custom IDs; should be changed to not use them at all maybe
#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif

//GUI context
struct UIState
{
	//uistate properties
	int mousex;
	int mousey;
	int mousedown;

	int hotitem;
	int activeitem;

	int kbfocus;
	int keyentered;
	int keymod;
	int keychar;
  
    int lastwidget;
	int lasthotitem;

	//these methods need to be called from the outside
	void imgui_prepare();
	void imgui_finish();
	void uiEvents(const sf::Event &aEvent);

	//these methods are called by the widgets
	int regionhit(const sf::FloatRect &rect);
	bool processkeyboard(int id);
	bool checkhot(int id, const sf::FloatRect & rect);

	sf::RenderWindow* renderer;
	
};

static sf::Clock uiClock;

//
///WIDGETS HERE
namespace widget
{
	int button(UIState & ui, int id, const sf::Vector2f &widthheight, const sf::Vector2f &pos, const char* text, float size = 12.0f);

	int buttonImage(UIState & ui, int id, int x, int y, sf::Sprite & sprite);
	
	//textfield for writing in: i.e. high score
	int textfield(UIState & ui, int id, const sf::FloatRect &rect, std::string &string);

	int scrollbar(UIState & ui, int id , int x, int y, int max, int &value, int width =16, int height=256);

	int checkbox(UIState & ui, int id, int x, int y, bool &value, const char* title= 0);
	
} //widget