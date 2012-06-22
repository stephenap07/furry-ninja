#include "imgui.h"

#include <SFML/Audio.hpp>

#include <iostream>
#include <string>
#include <cmath>

static float scale = .5f;

//functions

 int UIState::regionhit(const sf::FloatRect &rect)
{
	if ( rect.Contains(mousex, mousey) )
	 return 1;

   return 0;
}

 void UIState::imgui_prepare()
{
	hotitem = 0;
}

 void UIState::imgui_finish()
{
	if(mousedown == 0)
		activeitem = 0;
	else
	{
		if(activeitem == 0)
			activeitem = -1;
	}

	if(keyentered == sf::Keyboard::Tab)
		kbfocus = 0;

	keyentered = 0;
	keychar = 0;
}

 void UIState::uiEvents(const sf::Event &aEvent)
{
	switch(aEvent.Type)
	{
	case sf::Event::MouseMoved :
		{
			mousex = sf::Mouse::GetPosition(*renderer).x;
			mousey = sf::Mouse::GetPosition(*renderer).y;
		} break;

	case sf::Event::MouseButtonPressed:
		{
			if(aEvent.MouseButton.Button == sf::Mouse::Left)
			{
				mousedown = 1;
			}
		} break;

	case sf::Event::MouseButtonReleased:
		{
			if(aEvent.MouseButton.Button == sf::Mouse::Left)
				mousedown =0;
		} break;

	case sf::Event::KeyPressed:
		{
			keyentered = aEvent.Key.Code;
			keymod = aEvent.Key.Shift;
		}break;
	case sf::Event::TextEntered:
		{
			if( (aEvent.Text.Unicode & 0xFF80) == 0)
				keychar = aEvent.Key.Code;
		}break;
	}
}

//helper functions for widgets
 bool UIState::processkeyboard(int id)
{
	if(kbfocus == id)
		{
			switch(keyentered)
			{
				//if tab was pressed, lose keyboard focus for the next widget to pick it up
			case sf::Keyboard::Tab:
				{
					kbfocus = 0;

					if( keymod )
						kbfocus = lastwidget;

					keyentered = 0;
				}break;

			case sf::Keyboard::Return:
				return true; 
			}
		}

	return false;
}

 inline void RenderText(UIState &ui, const char* text, float x, float y, const sf::Color &color = sf::Color::White, unsigned size = 12)
 {
	 //new stuff from sfml2
	 sf::String str(text);
	 sf::Text txt; 
	 txt.SetString(str);
	 txt.SetCharacterSize(size);
	 //set center the string
	 txt.SetOrigin(txt.GetLocalBounds().Width/2, txt.GetLocalBounds().Height/2);
	 txt.SetPosition(x, y);

	 //drop shadow
	 txt.SetColor(sf::Color(0,0,0,255/2));
	 txt.Move(3,3);
	 ui.renderer->Draw(txt);

	 //draw the string
	 txt.SetColor(color);
	 txt.Move(-3,-3);
	 
	 ui.renderer->Draw(txt);
 }

bool UIState::checkhot(int id, const sf::FloatRect & rect)
{
	if(regionhit(rect))
	{
		hotitem =id;

		if(activeitem == 0 && mousedown)
		{
			activeitem = id;
		}

		return true;
	}

	else if(lasthotitem == id)
	{
		lasthotitem  = 0;
	}

	return false;
}

namespace widget
{
	int button(UIState &ui, int id, const sf::Vector2f &widthheight, const sf::Vector2f &pos, const char* text, float size)
	{
		//rectangle description
		float outline = 0;
		sf::Color outlinecolor(sf::Color(255,0,255));

		sf::Color fillColor = sf::Color(127, 127, 127, 127); 

		sf::RectangleShape button(widthheight);
		button.SetFillColor(fillColor); 
		button.SetOutlineColor(outlinecolor); 
		button.SetOutlineThickness(outline);
		button.SetPosition(pos);

		sf::FloatRect rect = button.GetGlobalBounds();

		if( ui.checkhot( id, button.GetGlobalBounds() ) )
		{
			if(ui.lasthotitem != id)
			{
				ui.lasthotitem = id;
			}
		}

		//checking keyboard focus
		if(ui.kbfocus == 0)
			ui.kbfocus = id;

		//if we have keyboard focus, draw the border
		if(ui.kbfocus == id)
		{
			outline = 2.5f;
			button.SetOutlineThickness(outline);
		}
	

		//render the button

		button.Move(4,4);

		ui.renderer->Draw(button);

		button.Move(-4,-4);

		if(ui.hotitem == id)
		{
			if(ui.activeitem == id)
			{
				button.Move(4,4);

				ui.renderer->Draw(button);
				RenderText(ui, text, rect.Left + rect.Width/2 + 4, rect.Top + rect.Height/2 + 4, sf::Color(255,255,255), size);
			}
			else 
			{
				button.SetFillColor(sf::Color(127,127,127,255));
				ui.renderer->Draw(button);
				RenderText(ui, text, rect.Left + rect.Width/2, rect.Top + rect.Height/2, sf::Color(255,255,255), size);
			}
		}

		else 
		{
			button.SetFillColor(sf::Color(127,127,127,255/2));

			ui.renderer->Draw(button);
			RenderText(ui, text,rect.Left + rect.Width/2, rect.Top + rect.Height/2, sf::Color(255,255,255,255/2), size);
		}

		//if enter was pressed and we have keyboard focus
		if(ui.processkeyboard(id))
			return 1;

		ui.lastwidget = id;

		//button was pressed
		if (ui.mousedown == 0
		&& ui.hotitem == id
		&& ui.activeitem == id)
			return 1;

		return 0;
	}

	int buttonImage(UIState &ui, int id, int x, int y, sf::Sprite & sprite)
	{
		sf::FloatRect rect(x, y, x+sprite.GetLocalBounds().Width, y+sprite.GetLocalBounds().Height);
		sprite.SetPosition(x,y);

		if(ui.checkhot(id, rect))
		{

			if(ui.lasthotitem != id)
			{
				ui.lasthotitem = id;
			}
		}

		sf::Color color = sprite.GetColor();
		color.a /= 2;
		sprite.SetColor(color);

		//sets the alpha to normal if item is hot
		if(ui.hotitem == id)
		{
			color.a *= 2;
			sprite.SetColor(color);
		}

		color.a = 255;
		ui.renderer->Draw(sprite);

		//turns the sprite back to normal
		sprite.SetColor(color);

		ui.lastwidget = id;

		//button was pressed
		if (ui.mousedown == 0
		&& ui.hotitem == id
		&& ui.activeitem == id)
			return 1;

		return 0;
	}

	//textfield for writing in: i.e. high score
	int textfield(UIState &ui, int id, const sf::Vector2f &widthheight, std::string &string)
	{
		static float elapsed_time = 0;

		float outline = 0;
		sf::Color outlinecolor(sf::Color(255,0,255));

		//set up shapes and strings
		sf::RectangleShape field ( widthheight );
		field.SetFillColor( sf::Color(127,127,127) );
		field.SetOutlineThickness( outline );
		field.SetOutlineColor( outlinecolor );

		sf::String str( string );
		sf::Text txt( str );

		sf::FloatRect rect = field.GetGlobalBounds();

		txt.SetScale(scale,scale);
		txt.SetPosition(rect.Left+8,rect.Top+6);

		//checking hotness
		if( ui.regionhit(rect) )
		{
			ui.hotitem = id;
			if(ui.activeitem == 0 && ui.mousedown)
				ui.activeitem = id;
		}

		//keyboard focus 
		if(ui.kbfocus == 0)
			ui.kbfocus = id;

		//if it has keyboard focus, set up outline 
		if(ui.kbfocus == id)
		{
			outline = 1.5f;
			field.SetOutlineThickness(outline);
		}

		//draw if it is active or hot
		if(ui.activeitem == id || ui.hotitem == id)
		{
			ui.renderer->Draw(field);
		}

		//or just normal...
		else 
		{
			field.SetFillColor(sf::Color(127,127,127, 255/2));
			if(ui.kbfocus == id)
				field.SetOutlineThickness(0.0f);

			ui.renderer->Draw(field);
		}

		//draw the text
		ui.renderer->Draw(txt);
		
		sf::String caret("_");
		sf::Text txt2(caret);

		txt2.SetScale(scale,scale);

		txt2.SetPosition(txt2.GetGlobalBounds().Width + txt2.GetGlobalBounds().Left, rect.Top+4);

		if(ui.kbfocus == id && (uiClock.GetElapsedTime().AsSeconds() - elapsed_time) > 0.75f) 
		{
			ui.renderer->Draw(txt2);
		}

		if( (uiClock.GetElapsedTime().AsSeconds() - elapsed_time) > 1.0f)
			elapsed_time = uiClock.GetElapsedTime().AsSeconds();

		int changed = 0;

		//if has keyboard focus
		if(ui.kbfocus == id)
		{
			switch(ui.keyentered)
			{
			case sf::Keyboard::Tab:
				{
					ui.kbfocus = 0;

					if(ui.keymod)
						ui.kbfocus = ui.lastwidget;

					ui.keyentered = 0;
				} break;
			case sf::Keyboard::Back:
				{
					if(string.length() > 0)
					{
						string.pop_back();
						changed = 1;
					}
				} break;
			}

			if(ui.keychar >= 32 && ui.keychar < 127 && txt.GetGlobalBounds().Width < rect.Width -18)
			{
				string.push_back(ui.keychar);
				changed = 1;
			}
		}

		if(ui.mousedown == 0 
			&& ui.hotitem == id
			&& ui.activeitem == id)

			ui.kbfocus = id;

		ui.lastwidget = id;

		return changed;
	}

	int scrollbar(UIState &ui, int id, int x, int y, int max, int &value, int width, int height)
	{
		//has a set height and width
		int ypos = ((height-width/2) * value)/max;

		if(ui.regionhit(sf::FloatRect(x, y, width, height)))
		{
			ui.hotitem = id;

			if(ui.activeitem == 0 && ui.mousedown)
				ui.activeitem = id;
		}

		sf::RectangleShape bar(sf::Vector2f(width, height+16));
		bar.SetPosition(x,y);
		bar.SetFillColor(sf::Color(10,10,10,255/2));

		sf::RectangleShape nub( sf::Vector2f(width/3 + width/2, width/3 + ypos + width/2) );
		nub.SetPosition(x+width/3,y+width/3 + ypos); 
		nub.SetFillColor(sf::Color(255,255,255,255/2));

		ui.renderer->Draw(bar);

		if(ui.activeitem == id || ui.hotitem == id)
		{
			nub.SetFillColor(sf::Color(255,255,255,255));
			ui.renderer->Draw(nub);
		}
		else ui.renderer->Draw(nub);

		if(ui.activeitem == id)
		{
			int mousepos = ui.mousey - (y + width/3);
			if(mousepos < 0) mousepos = 0;
			if(mousepos > height-1) mousepos = height-1;
			int v = (mousepos* max) / height;
			if(v != value)
			{
				value = v; 
				return 1;
			}
		}

		return 0;
	}

	int checkbox(UIState & ui, int id, int x, int y, bool &value, const char* title)
	{
		int width = 16;
		int height = 16;

		sf::FloatRect rect(x, y, width, height);

		if(ui.checkhot(id, rect))
		{

			if(ui.lasthotitem != id)
			{
				ui.lasthotitem = id;
			}
		}

		//color ranges
		sf::Uint8 colorvalue = 255/1.5;
		const sf::Color color1(colorvalue, colorvalue, colorvalue, colorvalue);
		const sf::Color color2(colorvalue, colorvalue, colorvalue, colorvalue*2);
		const sf::Color activeColor(255,255,40, colorvalue);

		sf::RectangleShape box( sf::Vector2f(rect.Left + rect.Width, rect.Height) );
		box.SetPosition(x,y);
		box.SetOutlineColor(color1);
		box.SetOutlineThickness(2.0f);

		box.SetFillColor(color2);

		//set color to a yellow color if the value is true
		if(value)
		{
			box.SetFillColor(activeColor);
		}

		ui.renderer->Draw(box);

		if(title)
		{
			sf::String string(title);
			sf::Text txt1(string);
			txt1.SetCharacterSize(11);

			txt1.SetPosition(rect.Left + rect.Width + 2, rect.Top);

			ui.renderer->Draw(txt1);
		}


		//mouse over = alpha value raises
		if(ui.hotitem == id || ui.activeitem == id)
		{
			box.SetFillColor( sf::Color(box.GetFillColor().r, box.GetFillColor().g, box.GetFillColor().b, 255) );
			ui.renderer->Draw(box);
		}

		if (ui.mousedown == 0
		&& ui.hotitem == id
		&& ui.activeitem == id)
		{
			value = !value; 
			return 1;
		}

		return 0;
	}

} //namespace widget