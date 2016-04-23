#include"mxmp.h"
#include<sstream>
#include<mxpng.h>
#include<algorithm>


namespace mp
{



	// intro screen class implementation


	mpIntroScreen::movingRect::movingRect()
	{
		point.x = 75;
		point.y = 230;
		point.z = 75;
		point.w = 25;
	
	}

	void mpIntroScreen::movingRect::moveRight()
	{
		point.x += 4;
		if(point.x > 640-75) point.x = 70;
	}

	mpIntroScreen::mpIntroScreen(std::string path, mx::mxSurface &main) : mxScreen(path, main) { }	


	void mpIntroScreen::alphaBlend(mx::mxSurface &surface1,mx::mxSurface &surface2 , float alpha)
	{
		//mx::mxSurfacePainter p1(&surface1), p2(&surface2), pfront(&front);

		mx::mxPainter p1(surface1),p2(surface2),pfront(front);
		
		//p1.paintLock();
		//p2.paintLock();
		//pfront.paintLock();

		p1.lock(); p2.lock(); pfront.lock();
		
		
		for(int i = 0; i < surface1.getSurface()->w; i++)
		{
			for(int z = 0; z < surface1.getSurface()->h; z++)
			{

				mx::Color col1, col2, setcolor;
				col1 = p1.getpixel(i,z);
				col2 = p2.getpixel(i,z);
				
				
				
				setcolor.color.colors[0] = static_cast<unsigned char>(col1.color.colors[0] + (1-alpha)*col2.color.colors[0]);
				setcolor.color.colors[1] = static_cast<unsigned char>(col1.color.colors[1] + (1-alpha)*col2.color.colors[1]);
				setcolor.color.colors[2] = static_cast<unsigned char>(col1.color.colors[2] + (1-alpha)*col2.color.colors[2]);
				//setcolor.color.colors[3] = static_cast<unsigned char>(col1.color.colors[3] + (1-alpha)*col2.color.colors[3]);
				pfront.setpixel(i,z, setcolor);
			}

		}

		p1.unlock(); p2.unlock(); pfront.unlock();
	}


	void mpIntroScreen::init()
	{


		loadSurface(background, "img/background.png");
		loadSurface(foreground, "img/start.png");


		std::ostringstream stream;
		stream  << "img/system.mxf";
		sys_font.loadFont(stream.str().c_str());


	
		setTimer(5);
		alpha_blend = 1.0f;

	}

	void mpIntroScreen::free()
	{
		stopTimer();
		background.cleanSurface();	
		foreground.cleanSurface();
		sys_font.freeFont();

	}

	void mpIntroScreen::render() // draw intro screen
	{

		front.copySurface(background, 0, 0);


		SDL_Rect rc = { bar.point.x, bar.point.y, bar.point.z, bar.point.w };
		SDL_FillRect(front, &rc, mx::Color::mapRGB(front, 0, 0, 100));

		std::ostringstream stream;

		stream << "(C) 2008 LostSideDead ";
		static int counter = 0;
		++counter;
		if(counter > 10) counter = 1;
		for(int i = 0; i < counter; i++) stream << ".";
		sys_font.printText(front, 240, 265, mx::Color::mapRGB(front, 255, 255, 255), stream.str());

		alphaBlend(front, foreground, alpha_blend);


		static int cnt = 0;
		static Uint32 white_color = mx::Color::mapRGB(front, 255,255,255);


		Uint32 color;
		
		if((++cnt)%2==0)  color = mx::Color::mapRGB(front, rand()%255, 0, rand()%255);
		else color = white_color; 


		sys_font.printSizedText(front, 200, 320, 62,62, color, "Press Space to play");


	}

	int mpIntroScreen::logic()
	{

		if(alpha_blend > 2)
			return 3;



		return keyscan();
	}


	int mpIntroScreen::keyscan()
	{

		Uint8 *keys;
		
		keys = SDL_GetKeyState(0);


		if(keys[SDLK_RETURN] || keys[SDLK_SPACE] || keys[SDLK_ESCAPE])
		{
			return 3;
		}

		return 0;

	}

	unsigned int mpIntroScreen::timerExecution(unsigned int timer)
	{

		alpha_blend += 0.001f;
		bar.moveRight();
		return timer;
	}


	// start game screen

	mpStartScreen::mpStartScreen(std::string path, mx::mxSurface &main) : mxScreen(path,main) 
	{ 
		menu.pushItem("Single Player");
		menu.pushItem("Multi-Player Local");
		menu.pushItem("Multi-Player Network");
		menu.pushItem("Options");
		menu.pushItem("High Scores");
		menu.cur_selected = 0;
		menus[SUB_SINGLE].pushItem(" Single Player Mode");
		std::ostringstream ss;
		ss << translateKey(false);
		menus[SUB_SINGLE].pushItem(ss.str(), 1);
		menus[SUB_SINGLE].cur_selected = 0;
		menus[SUB_LOCAL].pushItem(" Multiplayer Mode [Local]");
		menus[SUB_LOCAL].pushItem(" Player 1: KEYBOARD ");
		ss.str("");
		ss << " Player 2";
		if(mx::mxJoystick::joystickNum() > 0)
		{
			ss << ":  Joystick ";
		}
		else ss << ": KEYBOARD";
		menus[SUB_LOCAL].pushItem(ss.str());
		menus[SUB_LOCAL].cur_selected = 0;
		menus[SUB_NETWORK].pushItem(" Multiplayer Mode [Network] ");
		menus[SUB_NETWORK].pushItem(" LISTEN ");
		menus[SUB_NETWORK].pushItem(" ENTER IP ");
		menus[SUB_NETWORK].cur_selected = 0;
		menus[SUB_OPTIONS].pushItem(" Options ");
		menus[SUB_OPTIONS].pushItem(" Toggle Full Screen  Mode" );
		menus[SUB_OPTIONS].cur_selected = 0;
		menus[SUB_HIGHSCORE].pushItem(" Highscores ");
		menus[SUB_HIGHSCORE].pushItem(" View Highscores ");
		menus[SUB_HIGHSCORE].cur_selected = 0;
	}

	void mpStartScreen::init()
	{

		std::ostringstream stream;
		stream << "img/system.mxf";
		sys_font.loadFont(stream.str().c_str());
		loadSurface(background, "img/start.png");
		menu_active = -1;
		menu.cur_selected = 0;
	}
	

	void mpStartScreen::free()
	{
		background.cleanSurface();
		sys_font.freeFont();
	}
	

	void mpStartScreen::render()
	{
		front.copySurface(background, 0, 0);
		sys_font.printSizedText(front, 150, 15, 50, 50, mx::Color::mapRGB(front,255,255,255), "MX");
		sys_font.printSizedText(front, 420, 15, 50, 50, mx::Color::mapRGB(front, 200+rand()%55, 200+rand()%55, 200+rand()%255), "Mutatris");
		sys_font.printSizedText(front, 220, 380, 50,50,mx::Color::mapRGB(front, 255, 255, 200+rand()%255), "LostSideDead.com");

		SDL_Rect rc = { 75, 50, 250, 300 };
		SDL_FillRect(front, &rc, mx::Color::mapRGB(front, 0, 0, 0));
		SDL_Rect rcX = { 400, 50, 250, 300 };
		SDL_FillRect(front, &rcX, mx::Color::mapRGB(front,0, 0, 0));

		sys_font.printText(front, 80, 75, mx::Color::mapRGB(front, 255, 0, 0), "Game Modes");
		int p;
		for(p = 0; p < menu.index; p++)
			if(p == menu.cur_selected)
			sys_font.printText(front,80,90+(p*20),mx::Color::mapRGB(front,0,0,255),menu.items[p].name);
			else 
			sys_font.printText(front, 80, 90+(p*20), mx::Color::mapRGB(front, 255, 255,  225), menu.items[p].name);


		for(int i = 0; i < menus[menu.cur_selected].index; i++)
		{
			if(i == menus[menu.cur_selected].cur_selected)
			{

				if(menu.cur_selected == SUB_NETWORK && i == 2)
				{
					std::ostringstream ss;
					static int x = 0;
					std::string one_liner;
					if((++x)%2 == 0) one_liner += "_"; 

					ss << menus[menu.cur_selected].items[i].name << ": " << ip_address << one_liner;
					sys_font.printText(front, 410, 90+(i*20), mx::Color::mapRGB(front, 124,66,255), ss.str());

				}
				else
				sys_font.printText(front, 410, 90+(i*20), mx::Color::mapRGB(front,255, 0, 0), menus[menu.cur_selected].items[i].name);
			}
			else 
				sys_font.printText(front, 410, 90+(i*20), mx::Color::mapRGB(front,255,255, 255),menus[menu.cur_selected].items[i].name);

		}





	}

	std::string mpStartScreen::translateKey(bool flip)
	{
		std::ostringstream ss;
		
		if(SDL_NumJoysticks() == 0) {  ss << "Input: [ KEYBOARD ONLY ]"; return ss.str(); }

		ss << " Input: [ KEYBOARD";

		if(flip == true)
		{
			ss << " ONLY ]";
			return ss.str();
		}

		std::string sxt = SDL_JoystickName(0);
		if(sxt.length() > 7) sxt = sxt.substr(0, 7);

		ss << "/" << sxt << " ]";
		return ss.str();
	}
	

	void mpStartScreen::keypress(int key)
	{

		if(menu_active == SUB_NETWORK)
		{

			if(menus[menu_active].cur_selected == 2)
			{

				if((key >= '0' && key <= '9') || key == '.')
				{
					ip_address += key;
				}

				if(key == SDLK_BACKSPACE)
				{
					if(ip_address.length()>0) ip_address.erase(ip_address.end()-1);
				}

			}


		}


	}

	int mpStartScreen::keyscan()
	{

		Uint8 *buffer = SDL_GetKeyState(0);

		static unsigned int start_time = SDL_GetTicks();
		

		
		int diff_time = 0;

		diff_time = SDL_GetTicks()-start_time;

		if(diff_time > 140)
		{
			start_time = SDL_GetTicks();
		} else return 0;


		


		if(menu_active == -1 && buffer[SDLK_DOWN])
		{


			if(menu.cur_selected < menu.index-1)
			{
				
				menu.cur_selected++; 
				return 0;
			}

		}

		if(menu_active == -1 && buffer[SDLK_UP])
		{
			if(menu.cur_selected > 0)
			{
				
				menu.cur_selected--;
				return 0;
			}
		}


		if(menu_active == -1 && buffer[SDLK_RETURN])
		{
			menu_active = menu.cur_selected;
			return 0;
		}

		if(menu_active != -1 && buffer[SDLK_DOWN])
		{

			if(menus[menu_active].cur_selected < menus[menu_active].index-1)
			menus[menu_active].cur_selected++;
			return 0;

		}
		if(menu_active != -1 && buffer[SDLK_UP])
		{
			if(menus[menu_active].cur_selected > 0)
				menus[menu_active].cur_selected--;
			return 0;

		}


		if(menu_active != -1 && buffer[SDLK_RETURN])
		{		

			switch(menu_active)
			{
			case SUB_SINGLE:
				if(menus[menu_active].cur_selected == 0) return 420;
			break;
			case SUB_HIGHSCORE:
				if(menus[menu_active].cur_selected == 1)
				{
					return 0x1C;
				}

			break;

			case SUB_LOCAL:

				if(menus[menu_active].cur_selected >= 1 &&  menus[menu_active].cur_selected <= 2)
					return 0xEF;

				break;

			case SUB_NETWORK:
				if(menus[menu_active].cur_selected == 1)
				{
					return 0x1E;
				}
				if(menus[menu_active].cur_selected == 2)
				{
					return 0x1F;
				}

			break;

			case SUB_OPTIONS:

				if(menus[menu_active].cur_selected == 1)
				{

					SDL_WM_ToggleFullScreen( front );

				}

			
			break;

			case SUB_EXIT:
				return 1024;
			break;
			}
		}

		if(menu_active == SUB_SINGLE)
		{

			std::ostringstream ss;

			if(buffer[SDLK_LEFT])
			{

				if(menus[menu_active].cur_selected == 1)
				{

					if(menus[menu_active].items[menus[menu_active].cur_selected].index > 0)
					{
					
						menus[menu_active].items[menus[menu_active].cur_selected].index--;
						ss << translateKey(!static_cast<bool>(menus[menu_active].items[1].index));
						menus[menu_active].items[menus[menu_active].cur_selected].name = ss.str();

					}
				}

			}

			if(buffer[SDLK_RIGHT])
			{

				if(menus[menu_active].cur_selected == 1)
				{
					if(menus[menu_active].items[1].index < 1)
					{

						menus[menu_active].items[1].index++;
						ss << translateKey(!static_cast<bool>(menus[menu_active].items[1].index));
						menus[menu_active].items[menus[menu_active].cur_selected].name = ss.str();
						ss.str("");
					}

				}

			}

		}
		
		if(menu_active != -1 && menus[menu_active].cur_selected == 0 && buffer[SDLK_RETURN]) menu_active = -1;



		return 0;

	}

	int  mpStartScreen::logic()
	{
		return keyscan();
	}

	mpStartScreen::mpStartMenu::mpStartMenu()
	{
		index = 0;
	}

	void mpStartScreen::mpStartMenu::pushItem(std::string name, int in)
	{

		mpMenuItem item(name, ++index);
		item.index = in;
		items.push_back(item);
	}


	// highscores

	mpHighScore::mpHighScore(mxMut *mutx, std::string p, mx::mxSurface &m) : mxScreen(p, m), mut(mutx), scorez("scores.mxdb")
	{
		


	}

	void mpHighScore::reload_bin()
	{
		// copy contents of ET into a vector
		mx::mxEternal<s_score>::iterator i;

		v.erase(v.begin(), v.end());

		for(i=scorez->begin(); i!=scorez->end(); i++)
		{
			ETconv_data z;
			z.score = i->second.score;
			z.blocks_cleared = i->second.blocks_cleared;
			z.vname = i->first;
			v.push_back(z);
		}

		std::sort(v.begin(), v.end());
	
	}

	void mpHighScore::showScores(bool show)
	{
		if(show == true) input_mode = false;
		else input_mode = true;
	}

	void mpHighScore::init()
	{

		sys_font.loadFont("img/system.mxf");
		input_mode = true;
		reload_bin();
		loadSurface(background, "img/scores.png");
	}

	void mpHighScore::keypress(int key)
	{

		if(input_mode == false) return;
			

		if( (key >= 'a' && key <= 'z'))
		initz += key;

		if(key == SDLK_BACKSPACE)
		{

			if(!initz.empty())
			initz.erase(initz.end()-1);

		}

	}

	void mpHighScore::free()
	{

		sys_font.freeFont();
		background.cleanSurface();

	}

	void mpHighScore::render()
	{


		static Uint32 white_color = SDL_MapRGB(front, 255, 255, 255);
		static unsigned int z = 0;


		front.copySurface(background, 0, 0);

		std::ostringstream ss;

		ss << "Game Over Score: " << mut->grid.score << " Blocks Cleared: " << mut->grid.blocks_cleared << "\nPress Escape to Return";
		
		sys_font.printText(front, 30, 100, white_color, ss.str().c_str());

		ss.str("");

		if(input_mode == true) 
		{
	
			std::string str;

			if((++z%2)>0) str = "_";

			ss << "Enter your Initials: " << initz  << str;

			sys_font.printText(front, 30, 150,white_color , ss.str().c_str());
		}
		else
		{

			for(size_t i = 0; i < v.size(); i++)
			{

				if(i >= 10) break;
				
				ss.str("");

				ss << i << "): Score: " << v[i].score << ":" << v[i].blocks_cleared << " by " << v[i].vname;
				sys_font.printText(front, 30, 150+(i*15), white_color, ss.str().c_str());

			}
		

		}

		
		
		

	}
	int mpHighScore::logic()
	{
		Uint8 *keys = SDL_GetKeyState(0);

		static unsigned int start_time = SDL_GetTicks();
				
		int diff_time = 0;

		diff_time = SDL_GetTicks()-start_time;

		if(diff_time > 140)
		{
			start_time = SDL_GetTicks();
		} else return 0;

		
		

		if(input_mode ==  true)
		{

			if(mut->grid.score == 0) { input_mode = false; return 0; }


			if(keys[SDLK_SPACE] || keys[SDLK_RETURN])
			{


				if(initz.length() <= 1) return 0;
				input_mode = false;

				

				if(scorez[initz].score < mut->grid.score) 
				{
					scorez[initz].score = mut->grid.score;
					scorez[initz].blocks_cleared = mut->grid.score;
				}

				reload_bin();
				initz = "";

			}
		}
		else
		{

			if( keys[SDLK_ESCAPE] )
				return 3;

		}


		return 0;
	}


	mxmpWnd::mxmpWnd(std::string path, unsigned int flags, int w, int h) : mxWnd( mx::Size(w,h), mx::Size(720,480),  flags, SDL_LoadBMP("img/icon.bmp")), front(background), intro(path, front), start(path, front), game(path, front), scores(&game.new_game, path, front), multi(path, front), split(path, front)	
	{
		
		SDL_ShowCursor(SDL_FALSE);
		std::cout << "draw surface successfully created @720x480x32\n";
		render_screen = 0;
		setScreen(&intro);
		setTitle("Mutatris");
	}
	
	// no-throw guarantee ( hopefully :) )
	mxmpWnd::~mxmpWnd()
	{
		clrScreen();
	}

	void mxmpWnd::eventPassed(SDL_Event &e)
	{

		switch(e.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:
			
			if((render_screen == &game) && e.key.keysym.sym == SDLK_ESCAPE)
			{	setScreen(&scores); return;	 }

			if((render_screen == &scores))
			{
				scores.keypress(e.key.keysym.sym);
				return;
			}

			if((render_screen == &start))
			{
				start.keypress(e.key.keysym.sym);
			}

			if((render_screen == &multi))
			{

				if(e.key.keysym.sym == SDLK_ESCAPE) setScreen(&start);

				return;
			}
			if(render_screen == &split)
			{
				
				if(e.key.keysym.sym == SDLK_ESCAPE) setScreen(&start);
				return;

			}
			
			if(e.key.keysym.sym == SDLK_ESCAPE) quit();
			


			break;
		default:
			break;

		}

	}

	void mxmpWnd::renderScreen()
	{
	        int value = 0;

		// clear the screen
		front.Clear();


		if(render_screen != 0) {

			render_screen->render();
			value = render_screen->logic();
		}

		sizedFlip();
	
		if(value != 0) setScreenByIndex(value);
	}

	void mxmpWnd::setScreen(mx::mxScreen *scr)
	{

		clrScreen();
		render_screen = scr;
	
		try
		{
			render_screen->init();
		} 
		catch( ... )
		{
			std::cerr << "error on initaliziation of screen\n";
			clrScreen();
			render_screen = 0;
			throw;
		}

	}
	
	void mxmpWnd::setScreenByIndex(int value)
	{
		switch(value)
		{
		case 0:
			setScreen(&intro);
			break;
		case 3:
			 // second screen
			setScreen(&start);
			break;
		case 5:
			setScreen(&scores);
			break;
		case 0x1C:
			setScreen(&scores);
			scores.showScores(true);
			break;

		case 0x1E:

			multi.init_network("", false, 1045);
			setScreen(&multi);	
		break;

		case 0x1F:

			multi.init_network(start.ip_address, true, 1045);
			setScreen(&multi);

		break;
		case  0xEF:

			setScreen(&split);
			// new game - mode -

		break;

		case 420:

			if(start.menus[start.menu_active].items[1].index == 0)
				game.setKeyboardOnly(true);
			else if(start.menus[start.menu_active].items[1].index == 1)
				game.setKeyboardOnly(false);

			setScreen(&game);

			break;

		case 1024: quit(); break;
		}
	}

	void mxmpWnd::clrScreen()
	{
	
		if(render_screen != 0)
		{
			std::cout << "releasing screen data: " << (unsigned long) render_screen  << "\n";
			render_screen->free();

		}

		render_screen = 0;
	}
}

