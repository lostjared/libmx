#include "mxmp.h"

#define TIMEOUT_WAIT 5000

namespace mp
{

	mpMulti::mpMulti(std::string str, mx::mxSurface &main) : mxScreen(str, main)
	{


	}


	void mpMulti::init()
	{

		loadSurface(background, "img/gamebg.png");
		sys_font.loadFont("img/system.mxf");
		game_over = false;
		win = false;


	}

	void mpMulti::free()
	{

		background.cleanSurface();
		sys_font.freeFont();
		game_active = false;
		threadStop();
		game_socket.closeSocket();

	}

	void mpMulti::render()
	{

		front.copySurface(background, 0, 0);
		SDL_Rect rcx = { 10, 10, front.width()-20, 40 };


		if(con_type == true) 
		{
			renderMutatris(playerConnect);
			renderOpponent(playerListen);
			playerConnect.grid.merge_down();
			
		}
		else 
		{
			renderMutatris(playerListen);
			renderOpponent(playerConnect);
			playerListen.grid.merge_down();
		}			

		

		SDL_FillRect(front, &rcx, SDL_MapRGB(front, 0, 0, 0));

		if(game_over == true && connected == true && game_active == false)
		{

			std::ostringstream stream;

			stream << "Game over ";
			if(win == false) stream << " You Lose\n";
			else stream << "You win\n";

			sys_font.printText(front,15,15,SDL_MapRGB(front, rand()%255, rand()%255, rand()%255), stream.str());
			return;
		}




		if(con_type == false && con_failed == false && connected == false)
		{
			sys_font.printText(front, 15, 15, SDL_MapRGB(front, 255, 255, 255), "Listening .... ");
			return;
		}
		if(con_failed == true && connected == false)
		{
			sys_font.printText(front, 15, 15, SDL_MapRGB(front, 255, 0, 0), "Connection Error .... ");
			return;
		}

		std::ostringstream ss;
		ss << "Connected to: " << user_ip;

		if(con_type == true && connected == false)
		{
			sys_font.printText(front, 15, 15, SDL_MapRGB(front, 255, 255, 255), "Connecting ....");
			return;
		}
		
		
		
		if(connected == true)
			sys_font.printText(front, 15, 15, SDL_MapRGB(front, 255,255,255), ss.str());
	


	}

	int mpMulti::logic()
	{

		if(con_type == true && connected == true)
		{

			if(playerConnect.is_gameOver() == true)
			{
				game_over = true;
				win = false;
				playerConnect.stopTimer();
				playerConnect.grid.data[0][0] = 'G';
				
			}


		}
		else if(con_type == false && connected == true)
		{

			if(playerListen.is_gameOver() == true)
			{
				game_over = true;
				win = false;
				playerListen.stopTimer();
				playerListen.grid.data[0][0] = 'G';

			}

		}


		static Uint32 start_time = SDL_GetTicks();

		Uint32 diff=SDL_GetTicks()-start_time;
		if(diff > 140)
		{
			start_time=SDL_GetTicks();
		} else return 0;


		if(con_type == true)
			scanJoystick(playerConnect);
		else
			scanJoystick(playerListen);


		return 0;

	}

	void mpMulti::scanJoystick(mxMut &new_game)
	{

#ifndef SYSTEM_PS3
		if(mx::mxJoystick::joystickNum() > 0 && player1.isOpen())
		{

			if(player1.joystickGetButton(1))
				new_game.shiftColor();

			int value = player1.joystickGetAxis(0);

			if(value > 0)
				new_game.moveRight();
			if(value < 0)
				new_game.moveLeft();

			value = player1.joystickGetAxis(1);
			
			if(value > 0)
				new_game.moveDown();
		}
		
#else
		if(mx::mxJoystick::joystickNum() > 0 && player1.isOpen())
			{

				if(player1.joystickGetButton(1))
					new_game.shiftColor();

				int value = player1.joystickGetAxis(0);

				if(value > 0)
					new_game.moveRight();
				
				if(value < 0)
					new_game.moveLeft();
			
				value = player1.joystickGetAxis(1);
				
				if(value > 0)
					new_game.moveDown();
			}
#endif
		

		Uint8 *keys = SDL_GetKeyState(0);

		if(keys[SDLK_LEFT])
			new_game.moveLeft();
		
		if(keys[SDLK_RIGHT])
			new_game.moveRight();

		if(keys[SDLK_UP])
			new_game.shiftColor();

		if(keys[SDLK_DOWN])
			new_game.moveDown();

	}

	void mpMulti::init_network(std::string ip, bool con_type, int port)
	{
		game_socket.createSocket();
		this->ip = ip;
		this->con_type = con_type;
		this->port = port;
		game_active = true;
		game_over = false;
		win = false;
		threadRun();
	}

	void mpMulti::renderMutatris(mxMut &new_game)
	{

		static const int offset_x = 25, offset_y = 55;

		for(ssize_t i = 0; i < new_game.grid.grid_w; i++)
		{
			for(ssize_t z = 0; z < new_game.grid.grid_h; z++)
			{

				SDL_Rect rc = {offset_x+i*8,offset_y+z*8,6, 6};

				int num;

				if(i < new_game.grid.grid_w && z < new_game.grid.grid_h)
				num = new_game.grid.data[i][z];
				else return;
				
				SDL_FillRect(front, &rc, array_of_colors[num]);
			}
		}

		drawBlock(new_game.current,25,55);
		drawBlock(new_game.next, 325, 60);
	}

	void mpMulti::renderOpponent(mxMut &new_game)
	{

		static const int offset_x = 570;
		static const int offset_y = 150;

		sys_font.printText(front, 570, 120, SDL_MapRGB(front, 255, 255, 255), "Opponent");

		for(ssize_t i = 0; i < new_game.grid.grid_w; i++)
		{
			for(ssize_t z = 0; z < new_game.grid.grid_h; z++)
			{

				int num = 0;

				SDL_Rect rc = {offset_x+i*2, offset_y+z*2, 2, 2};
				if(i < new_game.grid.grid_w && z < new_game.grid.grid_h)
				{
					num = new_game.grid.data[i][z];

				} else return;

				SDL_FillRect(front, &rc, array_of_colors[num]);

			}

		}


	}


	void mpMulti::drawBlock(const gameBlock &current, const int offset_x, const int offset_y)
	{

		int pos_x = current.x;
		int pos_y = current.y;
		

		for(unsigned int z = 0; z < 4; z++)
		{
			blockType *t = (blockType *)&current.blocks[z];
			SDL_Rect rc = { ((t->x+pos_x)*8)+offset_x , ((t->y+pos_y)*8)+offset_y, 6 , 6};
			SDL_FillRect(front, &rc, array_of_colors[ t->index_color ]);
		}
	}



	int mpMulti::threadExec()
	{

		con_failed = false;
		connected = false;

		if(con_type == true)
		{
			if(!game_socket.connectTo(ip, port))
			{
				con_failed = true;
				return 0;
			}
			user_ip = ip;
			connected = true;

			playerConnect.newGame();
			playerListen.freeze(true);
			memset(playerListen.grid.data, 0, sizeof(playerListen.grid.data));
	
			
			while(game_active)  // game loop
			{

				pmut.lockMutex();

				SDL_Delay(TIMEOUT_WAIT);


				try
				{

					std::cout << "ping .. ";
					if( game_socket.Write((char*)playerConnect.grid.data, sizeof(playerConnect.grid.data)) == -1)
					{
						game_active = false;
						connected = false;
						con_failed = true;

					}

					if( game_socket.Read((char*)playerListen.grid.data, sizeof(playerListen.grid.data)) == -1)
					{

						game_active = false;
						connected = false;
						con_failed = true;
					}
					std::cout << " pong\n";

				
				}
				catch(mx::mxException<std::string> &e)
				{
					connected = false;
					game_active = false;
					con_failed = true;

				}
				catch (...) { }


				pmut.unlockMutex();
		
				if(playerListen.grid.data[0][0] == 'G')
				{
					game_active = false;
					game_over = true;
					win = true;
					playerListen.stopTimer();
				}

				if(playerConnect.grid.data[0][0] == 'G')
				{
					game_active = false;
					win = false;
					playerConnect.stopTimer();

				}

				
			
			}

			game_socket.closeSocket();

		}
		else 
		{
			game_socket.listenAt(port);
			std::string ip_a;

			mx::mxSocket lst_socket = game_socket.acceptNewSocket(ip_a);
			
			if(lst_socket.getsocket() < 0)
			{
				con_failed = true;
				return 0;
			}

			user_ip = ip_a;
			connected = true;

			playerListen.newGame();
			playerConnect.freeze(true);

			memset(playerConnect.grid.data, 0, sizeof(playerConnect.grid.data));			

			while(game_active) // game loop
			{
				SDL_Delay(TIMEOUT_WAIT);

				pmut.lockMutex();

				
				try
				{
					std::cout << "ping .. ";
					if ( lst_socket.Read((char*)playerConnect.grid.data, sizeof(playerConnect.grid.data)) == -1)
					{

						game_active = false;
						connected = false;
						con_failed = true;

						
					}

					if( lst_socket.Write((char*)playerListen.grid.data, sizeof(playerListen.grid.data)) == -1)
					{

						game_active = false;
						connected = false;
						con_failed = true;

					}

					
					std::cout << " pong \n";
				
					

				}
				catch (mx::mxException<std::string> &e)
				{

					game_active = false;
					connected = false;
					con_failed = true;
				}
				catch (...) { }

				pmut.unlockMutex();

				if(playerConnect.grid.data[0][0] == 'G')
				{

					game_over = true;
					win = true;
					game_active = false;
					playerConnect.stopTimer();
				}

				if(playerListen.grid.data[0][0] == 'G')
				{
					game_active = false;
					win = false;
					playerListen.stopTimer();
				}


			}

			lst_socket.closeSocket();

		}



		return 0;
	}






}

