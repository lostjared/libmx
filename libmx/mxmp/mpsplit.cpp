#include "mxmp.h"



namespace  mp
{

	mpSplit::mpSplit(std::string name, mx::mxSurface &main) : mxScreen(name, main), stick2(1), winner(0)
	{



	}

	void mpSplit::init()
	{

		loadSurface(background, "img/gamebg.png");
		sys_font.loadFont("img/system.mxf");

		player1.newGame();
		player2.newGame();
		winner = 0;

	}
	void mpSplit::free()
	{

		background.cleanSurface();
		sys_font.freeFont();

	}

	void mpSplit::render()
	{

		front.copySurface(background, 0, 0);
		drawGrid(25,25, player1);
		drawGrid(425,25, player2);

		drawBlock(player1.current,25,25);
		//drawBlock(player1.next, 325, 60);
		drawBlock(player2.current,425, 25);

		std::ostringstream ss;

		if(winner == 0) ss << "VERSUS MODE";
		else if(winner > 0 && winner <= 2) ss << "Player " << winner << " Wins";


		sys_font.printText(front, 300, 10, SDL_MapRGB(front.getSurface()->format, 255, 255, 255), ss.str());

		player1.grid.merge_down();
		player2.grid.merge_down();

	}


	int mpSplit::logic()
	{


		if(player1.is_gameOver() == true)
		{
			player1.stopTimer();
			player2.stopTimer();
			winner = 2;
		}
		else if(player2.is_gameOver() == true)
		{
			player2.stopTimer();
			player1.stopTimer();
			winner = 1;
		}


		static Uint32 start_time = SDL_GetTicks();

		Uint32 diff=SDL_GetTicks()-start_time;
		if(diff > 140)
		{
			start_time=SDL_GetTicks();
		} else return 0;


		Uint8  *keys = SDL_GetKeyState(0);

		if(keys[SDLK_UP])
			player1.shiftColor();

		if(keys[SDLK_LEFT])
			player1.moveLeft();

		if(keys[SDLK_RIGHT])
			player1.moveRight();

		if(keys[SDLK_DOWN])
			player1.moveDown();

		if(keys[SDLK_a])
			player2.moveLeft();
		if(keys[SDLK_w])
			player2.shiftColor();
		if(keys[SDLK_s])
			player2.moveDown();
		if(keys[SDLK_d])
			player2.moveRight();

		if(mx::mxJoystick::joystickNum() > 0 && stick.isOpen())
		{

				if(stick.joystickGetButton(1))
					player2.shiftColor();

				int value = stick.joystickGetAxis(0);

#ifdef __APPLE__

				if(value > 150)
					player2.moveRight();

				if(value < 50)
					player2.moveLeft();

				value = stick.joystickGetAxis(1);

				if(value > 150)
					player2.moveDown();


#else
				if(value > 0)
					player2.moveRight();


				if(value < 0)
					player2.moveLeft();

				value = stick.joystickGetAxis(1);

				if(value > 0)
					player2.moveDown();
#endif
		}

		if(mx::mxJoystick::joystickNum() > 0 && stick2.isOpen())
		{

				if(stick2.joystickGetButton(1))
					player1.shiftColor();

				int value = stick2.joystickGetAxis(0);


#ifdef __APPLE__


				if(value > 150)
					player1.moveRight();

				if(value < 50)
					player1.moveLeft();

				value = stick2.joystickGetAxis(1);

				if(value > 150)
					player1.moveDown();


#else

				if(value > 0)
					player1.moveRight();

				if(value < 0)
					player1.moveLeft();

				value = stick2.joystickGetAxis(1);

				if(value > 0)
					player1.moveDown();
#endif
		}


		return 0;
	}

	void mpSplit::drawGrid(int offset_x, int offset_y, mxMut &new_game)
	{

		for(ssize_t i = 0; i < new_game.grid.grid_w; i++)
		{
			for(ssize_t z = 0; z < new_game.grid.grid_h; z++)
			{

				SDL_Rect rc = {offset_x+i*4,offset_y+z*8, 4, 8};
				int num;
				if(i < new_game.grid.grid_w && z < new_game.grid.grid_h)
				num = new_game.grid.data[i][z];
				else return;

				SDL_FillRect(front, &rc, array_of_colors[num]);
			}
		}


	}

	void mpSplit::drawBlock(const gameBlock &current, const int offset_x, const int offset_y)
	{

		int pos_x = current.x;
		int pos_y = current.y;


		for(unsigned int z = 0; z < 4; z++)
		{
			blockType *t = (blockType *)&current.blocks[z];
			SDL_Rect rc = { ((t->x+pos_x)*4)+offset_x , ((t->y+pos_y)*8)+offset_y, 4 , 8};
			SDL_FillRect(front, &rc, array_of_colors[ t->index_color ]);
		}
	}


}

