#ifndef __MXMP_H_X
#define __MXMP_H_X
#ifdef HAVE_CONFIG_H
#include<config.h>
#endif


#include<mx.h>
#include<mxvector.h>
#include<mx_font.h>
#include<string>
#include<vector>
#include<iostream>
#include<mxeternal.h>
#include<mxsocket.h>
#include "mpmut.h"

namespace mp
{

	class mpIntroScreen : public mx::mxScreen, public mx::mxTimer  {


	public:
		mpIntroScreen(std::string path, mx::mxSurface &main);
		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();
		
		virtual unsigned int timerExecution(unsigned int timer);
		void alphaBlend(mx::mxSurface &surface1,mx::mxSurface &surface2 , float alpha);
		
		
	protected:
		mx::mxSurface background, foreground;
		mx::mxRegularFont sys_font;

		class movingRect {
		public:
			mx::mxiVector point;

			movingRect();
			void moveRight();
		};

		movingRect bar;
		float alpha_blend;
		int keyscan();
	};

	class mpStartScreen : public mx::mxScreen {
	public:
		mpStartScreen(std::string path, mx::mxSurface &main);
		
		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();
		void keypress(int key);



		int keyscan();
	

		typedef struct menuItems {
			std::string name;
			int id;
			int index;
			menuItems() { index = 0; }
			menuItems(std::string n, int i) { index = 0, name = n, i = i; }

		} mpMenuItem;


		class mpStartMenu {
		public:

			int index;
			int cur_selected;

			mpStartMenu();
			void pushItem(std::string name, int in_id=0);

			std::vector<mpMenuItem> items;

		protected:
			

		};
		
		int  menu_active;
		mpStartMenu menu, menus[5];
		std::string ip_address;

	protected:
		mx::mxSurface background;
		mx::mxRegularFont sys_font;
		enum { SUB_SINGLE, SUB_LOCAL, SUB_NETWORK, SUB_OPTIONS, SUB_HIGHSCORE, SUB_EXIT};
		std::string translateKey(bool flip);

	};

	extern unsigned int array_of_colors[];

	class mpHighScore;


	class mpGame : public mx::mxScreen {


	public:
		mpGame(std::string path, mx::mxSurface &main);
		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();
		friend class mpHighScore;
		friend class mxmpWnd;
		void setKeyboardOnly(bool yeah) { yeah == true ? (joy_on = false) : (joy_on = true); }

	protected:
		mxMut new_game;
		mx::mxRegularFont sys_font;
		mx::mxSurface background;
		mx::mxJoystick player1;
		void drawGrid(const int  offset_x, const int offset_y);
		void drawBlock(const gameBlock &block, const int offset_x, const int offset_y);
		bool joy_on;
		mx::mxEternal<int> totals;

	};

	class  mpSplit : public mx::mxScreen {

	public:
		mpSplit(std::string path, mx::mxSurface &main);
		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();

	protected:
		mxMut player1,  player2;
		mx::mxJoystick  stick, stick2;
		mx::mxSurface background;
		mx::mxRegularFont sys_font;
		int winner;

		void drawGrid(int offset_x, int offset_y, mxMut &new_game);
		void drawBlock(const gameBlock &current, const int offset_x, const int offset_y);

	};

	extern unsigned int array_of_colors[6];

	class mpMulti : public mx::mxScreen, public mx::mxThread {

	public:

		mpMulti(std::string path, mx::mxSurface &main);
		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();

		void init_network(std::string ip, bool con_type, int port);
		virtual int threadExec();

		void renderMutatris(mxMut &mut);
		void renderOpponent(mxMut &mut);
		void drawBlock(const gameBlock &current, const int offset_x, const int offset_y);
		void scanJoystick(mxMut &new_game);

	protected:
		mxMut playerListen, playerConnect;
		mx::mxMutex pmut;

		mx::mxRegularFont sys_font;
		mx::mxSurface background;
		mx::mxSocket  game_socket;
		mx::mxJoystick player1;

		std::string ip;
		int port;
		bool con_type;
		bool con_failed;
		bool connected;
		std::string user_ip;
		bool game_active;	
		bool game_over;
		bool win;	

	};

	class mpHighScore :  public mx::mxScreen {

	public:

		mpHighScore(mxMut *mut, std::string path, mx::mxSurface &main);

		virtual void init();
		virtual void free();
		virtual void render();
		virtual int logic();

		void keypress(int key);
		void showScores(bool show);
	protected:
		mx::mxRegularFont sys_font;
		mx::mxSurface background;
		mxMut *mut;
		

		class s_score {
		public:
			int score;
			int blocks_cleared;
		};

		
		mx::mxEternal<s_score> scorez;
		std::string initz;
		bool input_mode;

		struct ETconv_data {
				
			std::string vname;
			int score, blocks_cleared;
		};

		friend bool operator<(const ETconv_data &one, const ETconv_data &two)
		{

				if(one.score>two.score) return true; 
				return false;
		}
		
		std::vector<ETconv_data> v;
		void reload_bin();
	
	};


	class mxmpWnd : public mx::mxWnd {


	public:

		mxmpWnd(std::string path = "", unsigned int flags = 0, int w = 720, int h = 480);
		~mxmpWnd();

		virtual void renderScreen();
		virtual void eventPassed(SDL_Event &e);
	
		// set current screen
		void setScreen(mx::mxScreen *screen);
		void setScreenByIndex(int index);

	protected:
		mx::mxScreen *render_screen; // pointer to current screen
		mx::mxSurface &front;

		void clrScreen(); // free current screen
	private:
		// screens
		mpIntroScreen intro;
		mpStartScreen start;
		mpGame        game;
		mpHighScore   scores;
		mpMulti       multi;
		mpSplit       split;
		

	};

}

#endif


