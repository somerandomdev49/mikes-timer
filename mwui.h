/* Made by SomeRandomDev49. Please credit me in your work if you use this library :) */

#ifndef MWUI_H
#define MWUI_H

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <iterator>
#include <algorithm>
//#include "PicoSHA2.h"


/* sample dialog

sf::RenderWindow window(sf::VideoMode(width, height), title);

/ Components /

while (window.isOpen())
{
    sf::Event event;
    while (window.pollEvent(event))
    {
		
		/  / if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
			window.close();
		else if (event.type == sf::Event::Closed)
			window.close();
		/ Events /
    }
    window.clear();
    /  Drawing /
    window.display();
}

*/


namespace mwui
{
	// <-- CONFIG FILES --> //
	
	// thx, http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html.
	template <class Container>
	void split(const std::string& str, Container& cont, char delim = ' ')
	{
	    std::stringstream ss(str);
	    std::string token;
	    while (std::getline(ss, token, delim)) {
	        cont.push_back(token);
	    }
	}
	
	std::map<std::string, std::string> read_config(std::string src)
	{
		// std::cout << "Config -> " << src << std::endl;
		std::stringstream ss(src);
		std::string line;
		std::map<std::string, std::string> map;
		while(std::getline(ss, line))
		{
			// std::cout << "Config line -> " << line << std::endl;
			// TODO: trim.
			if(line.size() == 0) continue;
			// TODO: support headers.
			std::vector<std::string> v;
			split(line, v, ':');
			map[v[0]] = v[1];
		}
		return map;
	}
	std::string save_config(std::map<std::string, std::string> map)
	{
		std::stringstream ss;
		for(const auto &x : map)
		{
			ss << x.first << ':' << x.second << '\n';
		}
		return ss.str();
	}
	
	// <-- UITL --> //
	
	//template<class T>
	//T &last(std::vector<input_dialog_line_t> &container)
	//{
	//	return container[container.size()-1];
	//}
	
	std::string encrypt(const std::string &s)
	{
		//std::string hash_hex_str;
		//picosha2::hash256_hex_string(s, hash_hex_str);
		return s;
		//return hash_hex_str;
	}
	
	// thx, https://thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/.
	void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
	{
		// Get the first occurrence
		size_t pos = data.find(toSearch);
 
		// Repeat till end is reached
		while( pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}

	void save_file(std::string value, const std::string &path)
	{
		std::ofstream myfile;
		myfile.open(path);
		myfile << value << '\n';
		myfile.close();
	}
	std::string read_file(const std::string &path)
	{
		std::ifstream input(path);
		std::stringstream sstr;
		while(input >> sstr.rdbuf());
		// std::cout << "reading file -> " << value << std::endl;
		input.close();
		return sstr.str();
	}
	
	std::string format_time(float time)
	{
		int minutes = ((int)time) / ((int)60);
		int seconds = ((int)time) % ((int)60);
		int censecs = (time - (int)time) * 100;
		std::stringstream s;
		s
			<< std::setfill('0') << std::setw(2) << minutes << ":"
			<< std::setfill('0') << std::setw(2) << seconds << "."
			<< std::setfill('0') << std::setw(2) << censecs;
		return s.str();
	}
	
	
	
	
	
	
	
	
	
	

// //                                                      <-- UI -->                                                      // //
	
	struct
	{
		std::string fontName = "Retro Gaming.ttf";
		sf::Color bgColor = sf::Color::Black, fgColor = sf::Color::White, hlColor = sf::Color::Red;
	} styles;

	sf::Font font;

	bool init()
	{
		if(!font.loadFromFile("Retro Gaming.ttf"))
		{
			std::cerr << "Error loading font file!" << std::endl;
			return false;
		}
		return true;
	}

	void text_dialog(const std::string &t, const std::string &title, sf::Color fg = styles.fgColor, int width = 400, int height = 200)
	{
		sf::RenderWindow window(sf::VideoMode(width, height), title);
		std::string str;
	
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(32);
		text.setFillColor(styles.fgColor);
		text.setString(t);
	
	    while (window.isOpen())
	    {
	        sf::Event event;
	        while (window.pollEvent(event))
	        {
				/**/ if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
					window.close();
				else if (event.type == sf::Event::Closed)
					window.close();
	        }
		    window.clear();
		    window.draw(text);
		    window.display();
	    }
	}
	
	template<class item_t>
	struct list_dialog_prefs_t
	{
		const std::string &title;
		 std::vector<item_t> items;
		 std::function<std::string(const item_t&)> to_str;
		 std::function<bool(int, item_t&, const std::string&)> on_enter;
		 int width = 400;
		 int height = 200;
	};

	template<class item_t>
	void list_dialog(list_dialog_prefs_t<item_t> prefs)
	{
		sf::RenderWindow window(sf::VideoMode(prefs.width, prefs.height), prefs.title);
		std::vector<sf::Text> texts;
	
		int selected = 0;
		for(int i=0;i<prefs.items.size();i++)
		{
			sf::Text text;
			// set the font
			text.setFont(mwui::font);
			// set the character size
			text.setCharacterSize(32); // in pixels, not points!

			// set the color
			text.setFillColor(sf::Color::White);

			// set the text
			text.setString(prefs.to_str(prefs.items[i]));
			texts.push_back(text);
		}
		
	    while (window.isOpen())
	    {
	        sf::Event event;
	        while (window.pollEvent(event))
	        {
	            if (event.type == sf::Event::Closed)
	                window.close();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
					selected = (selected-1) % texts.size();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
					selected = (selected+1) % texts.size();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
					selected = (selected-1) % texts.size();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
					selected = (selected+1) % texts.size();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
					if(prefs.on_enter(selected, prefs.items[selected], prefs.to_str(prefs.items[selected]))) window.close();
	        }
			for(int i=0;i<texts.size();i++)
			{
				// set the text
				texts[i].setString(prefs.to_str(prefs.items[i]));
			}
		
		    window.clear();
		
			/// <== == == drawing == == ==> ///
		
			int d = 0;

			// <-- texts before selected --> //
			for(int i=0;i<selected;i++)
			{
				texts[i].setFillColor(sf::Color::White);
				texts[i].setPosition(0, d);
				d += 48;
				window.draw(texts[i]);
			}
			// <-- selected text --> //
			{
				texts[selected].setFillColor(sf::Color::Red);
				texts[selected].setPosition(0, d);
				d += 48;
				window.draw(texts[selected]);
			}
			// <-- texts after selected --> //
			for(int i=selected+1;i<texts.size();i++)
			{
				texts[i].setFillColor(sf::Color::White);
				texts[i].setPosition(0, d);
				d += 48;
				window.draw(texts[i]);
			}
	//
		    window.display();
	    }
	}

	struct input_result_t
	{
		std::string text;
		bool success;
	};
	
	float get_char_width(const sf::Font &font, uint32_t c, int size = 32)
	{
		return font.getGlyph(c, size, false).advance;
	}

	float get_chars_width(const std::string &str, int cursor)
	{
		float w = 0.0;
		for(int i=0;i<cursor;i++)
		{
			w += get_char_width(font, str[i]);
		}
		return w;
	}


	// struct input_dialog_chunk_t
// 	{
// 		std::string chunk;
// 	};
	struct input_dialog_line_t
	{
		std::string text;
		float width()
		{
			return get_chars_width(text, text.size());
		}
		int cursor;
	};

#define last(c) c[c.size()-1]
#define last_line lines[line_index]
#define set_line_index_last line_index = lines.size() - 1
	;
	std::string get_string_from_lines(const std::vector<input_dialog_line_t> &lines)
	{
		std::string res = "";
		for(int i=0;i<lines.size()-1;i++) // size-1 because we don't want to add newline at the end.
		{
			res += lines[i].text + '\n';
		}
		res += last(lines).text;
		return res;
	}
	
	
	struct input_prefs_t {
		std::string title = "input", default_result = "";
		bool allow_empty = true;
		int width = 400, height = 200;
		bool multiline = false, debug = false;
	} default_input_prefs;

	
	input_result_t input_dialog(input_prefs_t prefs)
	{
		sf::RenderWindow window(sf::VideoMode(prefs.width, prefs.height), prefs.title);
		std::vector<input_dialog_line_t> lines;
		lines.push_back({"", 0});
		int line_index = 0;
		set_line_index_last;
		//int cursor = 0;
		sf::RectangleShape cursor_rect;
		cursor_rect.setSize(sf::Vector2f(5, 32));
		cursor_rect.setFillColor(sf::Color::White);
		float cursorX = 0;
		float cursorY = 0;
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(32);
		text.setFillColor(styles.fgColor);
	
	    while (window.isOpen())
	    {
	        sf::Event event;
	        while (window.pollEvent(event))
	        {
	            /**/ if (event.type == sf::Event::Closed)
	                window.close();
				else if (event.type == sf::Event::TextEntered)
				{
				    // Handle ASCII characters only
				    if (event.text.unicode < 128)
				    {
						char input_char = static_cast<char>(event.text.unicode);
						if(input_char != 8) // NOT BACKSPACE !
						{
							// std::cout << "input -> " << input_char << std::endl;
							if(input_char == 10)
							{
								lines.push_back({"", 0});
								cursorX = 0;
								cursorY += font.getLineSpacing(32);
								set_line_index_last;
							}
							else
							{
								last_line.text =
									last_line.text.substr(0, last_line.cursor) +
									input_char +
									last_line.text.substr(last_line.cursor, last_line.text.size());
								last_line.cursor++;
							}
							// std::cout << "last_line -> " << last_line.text << std::endl;
						}
						else // BACKSPACE !
						{
							if(last_line.text.size() < 1 && lines.size() > 1)
							{
								if(lines.size() > 1) lines.pop_back();
								line_index--;
								cursorX = last_line.width();
								cursorY -= font.getLineSpacing(32);
							}
							else if(last_line.text.substr(0, last_line.cursor).size() > 0)
							{
								last_line.text =
									last_line.text.substr(0, last_line.cursor-1) +
									last_line.text.substr(last_line.cursor, last_line.text.size());
								last_line.cursor--;
							}
						}
				        //text.setString(last<input_dialog_line_t>(lines).text);
				    }
				}
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter) && (event.key.shift || (!prefs.multiline)))
				{
					if( (!prefs.allow_empty)  &&  last_line.text.size() == 0 ) return {prefs.default_result, true};
					else return {get_string_from_lines(lines), true};
				}
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
				{
					if(last_line.cursor > 0)
						last_line.cursor--;
					else if(line_index > 0)
					{
						line_index = line_index > 0 ? --line_index : 0;
						cursorY -= font.getLineSpacing(32);
					}
				}
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
				{
					if(last_line.cursor == last_line.text.size() && line_index < lines.size()-1)
					{
						line_index++;
						cursorX = 0;
						cursorY += font.getLineSpacing(32);
					}
					else if(last_line.cursor < last_line.text.size())
					{
						last_line.cursor++;
					}
				}
	        }
			if(prefs.debug) window.setTitle(std::to_string(last_line.cursor) + "   /  [" + std::to_string(line_index) + "] " + std::to_string(last_line.text.size()) + "   (" + last_line.text + ")");
			text.setString(get_string_from_lines(lines));
			cursorX = (last_line.text.size() > 0) ? get_chars_width(last_line.text, last_line.cursor) : 0;
			cursor_rect.setPosition(cursorX, cursorY);
		    window.clear();
		    window.draw(text);
			window.draw(cursor_rect);
		    window.display();
	    }
		return {prefs.default_result, false};
	}
	
	input_result_t password_dialog(const std::string &title, std::string d = "", char pass_char = '*', bool allow_empty = true, int width = 400, int height = 200)
	{
		sf::RenderWindow window(sf::VideoMode(width, height), title);
		std::string str;
	
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(32);
		text.setFillColor(styles.fgColor);
	
	    while (window.isOpen())
	    {
	        sf::Event event;
	        while (window.pollEvent(event))
	        {
	            /**/ if (event.type == sf::Event::Closed)
	                window.close();
				else if (event.type == sf::Event::TextEntered)
				{
				    // Handle ASCII characters only
				    if (event.text.unicode < 128)
				    {
				        str += pass_char;//static_cast<char>(event.text.unicode);
						if(event.text.unicode == 8)
							str = str.substr(0, str.size()-2);
				        text.setString(str);
				    }
				}
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
				{
					if( (!allow_empty)  &&  str.size() == 0 ) return {d, true};
					else return {str, true};
				}
	        }
		    window.clear();
		    window.draw(text);
		    window.display();
	    }
		return {d, false};
	}
	
	// :P
	input_result_t input_multiline_dialog(input_prefs_t p)
	{
		p.multiline = true;
		return input_dialog(p);
	}
	
	enum struct image_dialog_type_t {
		resize_image,
		resize_frame,
		as_is
	};
	
	struct image_dialog_prefs_t
	{
		const std::string &title;
		const sf::Texture *texture;
		image_dialog_type_t type;
		int width = 400, height = 400;
	};
	
	void image_dialog(image_dialog_prefs_t prefs)
	{
		int width = prefs.width;
		int height = prefs.height;
		
		sf::RectangleShape sprite;
		//sprite.setSize(sf::Vector2f(texture->getSize().x, texture->getSize().y));
		sprite.setTexture(prefs.texture);
		
		// std::cout << "set sprite texture" << std::endl;
		
		switch(prefs.type)
		{
			case(image_dialog_type_t::resize_image):
			{
				break;
			}
			case(image_dialog_type_t::resize_frame):
			{
				width = prefs.texture->getSize().x;
				height = prefs.texture->getSize().y;
				break;
			}
			case(image_dialog_type_t::as_is):
			{
				break;
			}
		}
		
		// std::cout << "set type" << std::endl;
		
		sprite.setSize(sf::Vector2f(width, height));
		sf::RenderWindow window(sf::VideoMode(width, height), prefs.title);
		
		// std::cout << "create window" << std::endl;

		while (window.isOpen())
		{
		    sf::Event event;
		    while (window.pollEvent(event))
		    {
		
				/**/ if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
					window.close();
				else if (event.type == sf::Event::Closed)
					window.close();
				/* Events */
		    }
		    window.clear();
		    window.draw(sprite);
		    window.display();
		}
		
	}
	struct image_dialog_path_prefs_t : public image_dialog_prefs_t
	{
		const std::string &path;
	};
	image_dialog_path_prefs_t image_dialog_path_prefs(const std::string &title,
													  const std::string &path,
													  image_dialog_type_t type,
													  int width = 400,
													  int height = 400)
	{
  		return {title, nullptr, type, width, height, path};
  	}
	
	void image_dialog_path(image_dialog_path_prefs_t prefs)
	{
		auto texture = new sf::Texture;
		if (!texture->loadFromFile(prefs.path))
		    return;
		// std::cout << "load texture" << std::endl;
		prefs.texture = texture;
		image_dialog(prefs);
	}
} // namespace mwui;
#endif // MWUI_H