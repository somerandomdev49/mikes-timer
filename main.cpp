/* Made by SomeRandomDev49. Please credit me in your work if you use this thing :) */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <thread>
#include "mwui.h"


// TODO: remove duplicates in event handling somehow :)


// first real timer check -> dinner at chadeyka_nastya(8 feb, 2020) -> 07:11.38;

// mike's time world record -> 21:57.09 (9 feb, 2020, 22:47)  vvv <- was before.
// mike's time world record -> 10:01.00

struct
{
	std::string timesFile;
	std::string defaultDescription;
	std::string defaultFile;
	std::string quicksaveFileFormat;
	std::string password;
} settings;

// std::string get_default_description(float time)
// {
// 		std::string desc = settings.defaultDescription;
// 		findAndReplaceAll(desc, "$TIME", format_time(time));
// 	    std::time_t t = std::time(nullptr);
// 	    char mbstr[32];
// 	    if (std::strftime(mbstr, sizeof(mbstr), "%d-%m-%y", std::localtime(&t))) {
// 	        findAndReplaceAll(desc, "$DATE", mbstr);
// 	    }
// 		// "%d-%m-%y"
// 		return desc;
// }

void load_settings_file()
{
	auto settings_text = mwui::read_file("settings.mcf");
	auto s = mwui::read_config(settings_text);
	settings.timesFile = s["------------timesFile"];
	settings.defaultDescription = s["defaultDescription"];
	settings.defaultFile = s["defaultFile"];
	settings.quicksaveFileFormat = s["quicksaveFileFormat"];
	settings.password = s["------------password"];
}

void save_settings_file()
{
	std::map<std::string, std::string> map;
	map["timesFile"] = settings.timesFile;
	map["defaultDescription"] = settings.defaultDescription;
	map["defaultFile"] = settings.defaultFile;
	map["quicksaveFileFormat"] = settings.quicksaveFileFormat;
	map["password"] = settings.password;
	mwui::save_file(mwui::save_config(map), "settings.mcf");
}

void save_dialog(float time)
{
	auto desc = mwui::input_dialog({.title="Description", .default_result=settings.defaultDescription, .allow_empty=false});
	if(desc.success)
	{
		auto path = mwui::input_dialog({.title="Path", .default_result=settings.defaultDescription, .allow_empty=false});
		if(path.success)
		{
			mwui::save_file(desc.text + ":\n" + mwui::format_time(time), path.text);
		}
	}
}

struct prop_t
{
	std::string name, value;
	bool can_be_empty = false;
};

void pref_dialog()
{
	bool pass_changed = false;
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Preferences");
	std::vector<prop_t> prefs;
	std::vector<sf::Text> texts;
	
	
	prefs.push_back({ "times file", settings.timesFile });
	prefs.push_back({ "default description", settings.defaultDescription, true });
	prefs.push_back({ "default file", settings.defaultFile });
	prefs.push_back({ "quicksave file format", settings.quicksaveFileFormat });
	prefs.push_back({ "password", settings.password });
	
	int selected = 0;
	for(int i=0;i<prefs.size();i++)
	{
		sf::Text text;
		// set the font
		text.setFont(mwui::font);

		// set the character size
		text.setCharacterSize(32); // in pixels, not points!

		// set the color
		text.setFillColor(sf::Color::White);

		// set the text
		text.setString(prefs[i].name + " : " + prefs[i].value);
		texts.push_back(text);
	}
	
	// std::cout << "Pref size -> " << prefs.size() << std::endl;
	// std::cout << "Text size -> " << texts.size() << std::endl;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
//			selected = (selected+1) % texts.size();
// 			selected = (selected-1) % texts.size();
			
            if (event.type == sf::Event::Closed)
                window.close();
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
				selected = texts.size() != 0 ? ((selected-1) % texts.size()) : 0;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
				selected = texts.size() != 0 ? ((selected+1) % texts.size()) : 0;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
			{
				//if(prefs[selected].name == "password")
				//{
				//	auto d = mwui::input_dialog(prefs[selected].name, prefs[selected].value, true, 800);
				//	if(d.success)
				//	{
				//		std::cout << "D.SUCCESS -> ENCRYPT(" << d.text << ") = " <<  mwui::encrypt(d.text);
				//		prefs[selected].value = mwui::encrypt(d.text);
				//	}
				//	else
				//		prefs[selected].value = d.text;
				//}
				//else
				prefs[selected].value = mwui::input_dialog({.title=prefs[selected].name, .default_result=prefs[selected].value, .allow_empty=prefs[selected].can_be_empty, .width=800}).text;
			}
        }
		for(int i=0;i<texts.size();i++)
		{
			// set the text
			texts[i].setString(prefs[i].name + " : " + prefs[i].value);
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
	settings.timesFile = prefs[0].value;
	settings.defaultDescription = prefs[1].value;
	settings.defaultFile = prefs[2].value;
	settings.quicksaveFileFormat = prefs[3].value;
	settings.password = prefs[4].value;
}

// void init_timer_saved_window()
// {
// 	//sf::Thread(run_timer_saved_window).launch();
// }

struct timer_capture_t
{
	float time;
	float captureTime;
};

void timer_dialog()
{
	sf::Text text;

	bool shouldUpdate = false;
	
	sf::Time dt;
	float t = 0.0;
	float et = 0.0;

	// select the font
	text.setFont(mwui::font); // font is a sf::Font

	// set the character size
	text.setCharacterSize(72); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);
	// set the string to display
	text.setString("55:66.77");
	//window.setSize(sf::Vector2u(text.getLocalBounds().width, text.getLocalBounds().height));
	
	// the text is above because i used it's localBlounds to set the window's size
	
	// std::cout << text.getLocalBounds().width << "  <->  " << text.getLocalBounds().height << std::endl;
    sf::RenderWindow window(sf::VideoMode(380 + 10, 100 + 800), "Mike's Timer.");

	// text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	std::cout << std::endl;
	sf::Clock clock; // starts the clock
	
	std::vector<timer_capture_t> captures;
	std::vector<sf::Text> capture_texts;
	
	//capture_texts.push_back(sf::Text());
	
	int selected = 0;
	
	// vvvvvv  <-  not in window ui style. ( mwui -> mike's window user interface )
	
	// sf::RectangleShape splitLine;
	// splitLine.setPosition(380 + 10 + 5, 0);
	// splitLine.setSize(sf::Vector2f(5, 100 + 800));
	
	bool timerRunning = true;
	
    while (window.isOpen())
    {
		if(et > 0.01f)
		{
			t += 0.01f;
			et = 0;
		}
        sf::Event event;
        while (window.pollEvent(event))
        {
            /**/ if (event.type == sf::Event::Closed)
                window.close();
			else if (event.type == sf::Event::MouseButtonReleased)
				timerRunning = !timerRunning;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::R))
			{
				t = 0;
				shouldUpdate = true;
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
				save_dialog(t);
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
				timerRunning = !timerRunning;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P))
			{
				//std::cout << settings.password << std::endl;
				//auto d = mwui::password_dialog("enter the password", "", '*', true);
				//std::cout << mwui::encrypt(d.text) << std::endl;
				//if(mwui::encrypt(d.text) == settings.password)
				pref_dialog();
				//else
				//	mwui::text_dialog("incorrect password!", "mike's timer", sf::Color::Red);
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
			{
				// std::cout << "heh" << std::endl;
				captures.push_back({t, -.0f});
				sf::Text text;
				text.setFont(mwui::font);
				text.setCharacterSize(32);
				text.setFillColor(sf::Color::White);
				text.setString(mwui::format_time(t));
				capture_texts.push_back(text);
				// std::cout << "heh2" << std::endl;
				
				for(const auto &x : captures)
				{
					std::time_t tm = std::time(nullptr);
					char mbstr[32];
					std::string d = "DATE";
			  	    if (std::strftime(mbstr, sizeof(mbstr), "%d-%m-%y", std::localtime(&tm))) {
			  	        d = std::string(mbstr);
			  	    }
					std::string s = settings.quicksaveFileFormat;
					mwui::findAndReplaceAll(s, "$DATE", d);
					// std::cout << t << " -> " << mwui::format_time(t) << std::endl;
					// std::cout << "heh3" << std::endl;
					mwui::save_file(settings.defaultDescription + ":\n" + mwui::format_time(t), s);
					// std::cout << "heh4" << std::endl;
				}
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Tab))
			{
				captures.push_back({t, -.0f});
				sf::Text text;
				text.setFont(mwui::font);
				text.setCharacterSize(32);
				text.setFillColor(sf::Color::White);
				text.setString(mwui::format_time(t));
				capture_texts.push_back(text);
			}
			
			// CAPTURES
			
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
				selected = capture_texts.size() != 0 ? ((selected-1) % capture_texts.size()) : 0;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
				selected = capture_texts.size() != 0 ? ((selected+1) % capture_texts.size()) : 0;
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
			{
				// float t_ = t;
				if(captures.size() != 0)
				mwui::list_dialog<std::string>({
					mwui::format_time(t),
					{"Delete", "Save"},
					[&](const std::string &i){return i;},
					[&](int idx, std::string &i, const std::string &v){ // captures were t, selected, &captures, &capture_texts.
						// std::cout << idx << " -> " << i << " -> " << v << std::endl;
						if(idx == 0)
						{
							captures.erase(captures.begin() + selected);
							capture_texts.erase(capture_texts.begin() + selected);
							if(selected == captures.size() && captures.size() != 0)
								selected--;
							else if (captures.size() == 0)
								selected = 0;
						}
						else
							save_dialog(captures[selected].time);
						return true;
					}
				});
				// std::cout << "selected=" << selected << ", " << "#capture_texts=" << capture_texts.size() << ", #captures=" << captures.size() << std::endl;
			}
		}
#ifdef TERMINAL_OUT
		std::cout << t << "->" << mwui::format_time(t) << "\r";
#endif
		if(timerRunning || shouldUpdate)
		{
			text.setString(mwui::format_time(t));
			shouldUpdate = false;
		}
	    window.clear();
	    window.draw(text);
		
		int d = 48*2;

		// <-- texts before selected --> //
		for(int i=0;i<selected;i++)
		{
			capture_texts[i].setFillColor(sf::Color::White);
			capture_texts[i].setPosition(0, d);
			d += 48;
			window.draw(capture_texts[i]);
		}
		// <-- selected text --> //
		if(capture_texts.size() != 0)
		{
			
			capture_texts[selected].setFillColor(sf::Color::Red);
			capture_texts[selected].setPosition(0, d);
			d += 48;
			window.draw(capture_texts[selected]);
		}
		// <-- texts after selected --> //
		for(int i=selected+1;i<capture_texts.size();i++)
		{
			capture_texts[i].setFillColor(sf::Color::White);
			capture_texts[i].setPosition(0, d);
			d += 48;
			window.draw(capture_texts[i]);
		}
		
	    window.display();
		if(timerRunning)
		{
			et += dt.asSeconds();
			dt = clock.restart();
		}
    }
}


void dialog(const std::string &title, int width = 400, int height = 200)
{
	sf::RenderWindow window(sf::VideoMode(width, height), title);

	/* Components */

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
	    /* Drawing */
	    window.display();
	}
}

void mwui_tests()
{
	mwui::list_dialog<std::string>({
		"mwui tests",
		{"input", "multiline", "password", "text", "list", "image", "close"},
		[&](const std::string &item){return item;},
		[&](int idx, std::string &item, const std::string &value) {
			switch(idx)
			{
				case(0):
				{
					auto r = mwui::input_dialog({.title = value, .default_result = "default"});
					//std::cout << "dialog test -> " << r.success << " -> " << r.text;
					break;
				}
				case(1):
				{
					auto r = mwui::input_multiline_dialog({.title = value, .default_result = "default"});
					//std::cout << "dialog test -> " << r.success << " -> " << r.text;
					break;
				}
				case(2):
				{
					auto r = mwui::password_dialog(value, "default");
					//std::cout << "dialog test -> " << r.success << " -> " << r.text;
					break;
				}
				case(3):
				{
					mwui::text_dialog(value, "default"); 
					break;
				}
				case(4):
				{
					break;
				}
				case(5):
				{
					mwui::list_dialog<std::string>({
						"mwui tests",
						{"resize image", "resize frame", "as is", "close"},
						[&](const std::string &item){return item;},
						[&](int idx, std::string &item, const std::string &value) {
							switch(idx)
							{
								case(0):
								{
									auto prefs = mwui::image_dialog_path_prefs(value, "test_image.png", mwui::image_dialog_type_t::resize_image);
									// std::cout << "init image prefs!" << std::endl;
									mwui::image_dialog_path(prefs); 
									break;
								}
								case(1):
								{
									auto prefs = mwui::image_dialog_path_prefs(value, "test_image.png", mwui::image_dialog_type_t::resize_frame);
									// std::cout << "init image prefs!" << std::endl;
									mwui::image_dialog_path(prefs);
									break;
								}
								case(2):
								{
									auto prefs = mwui::image_dialog_path_prefs(value, "test_image.png", mwui::image_dialog_type_t::as_is);
									// std::cout << "init image prefs!" << std::endl;
									mwui::image_dialog_path(prefs); 
									break;
								}
								case(3):
								{
									return true;
								}
								default: break;
							}
							return false;
						}, 500, 800
					});	
					// mwui::image_dialog(value, "test_image.png", ); 
					break;
				}
				case(6):
				{
					return true;
				}
			}
			return false;
		}, 200, 800
	});	
}

int main(int argc, char *argv[])
{
	if(!mwui::init()) return -1;
	std::string x = argc>1 ? argv[1] : "";
	if(x == "-test")  mwui_tests();
	else {
		
		load_settings_file();
		timer_dialog();
		//mwui_tests();
	
		save_settings_file();
	}
    return 0;
}
