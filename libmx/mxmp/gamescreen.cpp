#include "mxmp.h"
#define MAX_COLORS 6

#include<sstream>


namespace mp
{

	unsigned int array_of_colors[MAX_COLORS];

	// game screen implementation
	mpGame::mpGame(std::string path, mx::mxSurface &main) : mxScreen(path, main), totals("totals.mxdb")
	{
		array_of_colors[0] = SDL_MapRGB(front, 0, 40, 100);
		array_of_colors[1] = SDL_MapRGB(front, 255, 0, 0);
		array_of_colors[2] = SDL_MapRGB(front, 0, 0, 255);
		array_of_colors[3] = SDL_MapRGB(front,200,150,0);
		array_of_colors[4] = SDL_MapRGB(front,155,155,155);
		array_of_colors[5] = SDL_MapRGB(front, 0,255,255);
	}

	void mpGame::init()
	{
		new_game.newGame();
		new_game.grid.clear();
		sys_font.loadFont("img/system.mxf");
		loadSurface(background, "img/gamebg.png");
	}

	void mpGame::free()
	{

		sys_font.freeFont();
		background.cleanSurface();


	}

	void mpGame::render()
	{

		//SDL_FillRect(front, 0,  SDL_MapRGB(front, 0,0,0));
		front.copySurface(background, 0, 0);
		drawGrid(10,10);
		drawBlock(new_game.current,10,10);
		drawBlock(new_game.next, 325, 50);

		sys_font.printText(front,575,23, SDL_MapRGB(front, 255,255,255), "Next Block");

		std::ostringstream ss;
		ss << "Score: " << new_game.grid.score;
		sys_font.printText(front,560, 170, SDL_MapRGB(front, 255,255,255), ss.str());
		ss.str("");

		ss << "Cleared: " << new_game.grid.blocks_cleared;
		sys_font.printText(front,560, 190, SDL_MapRGB(front, 255, 255, 255), ss.str());

		SDL_Rect rc = { 10, front.height()-50, front.width()-180, 40 };
		SDL_FillRect(front, &rc, SDL_MapRGB(front, 0, 0, 0));
		ss.str("");
		ss << "Total Blocks Cleared:. " << totals["totald"] << " Total Score: " << totals["score"];
		sys_font.printText(front, 15, 442, SDL_MapRGB(front, 255, 255, 255), ss.str());

		new_game.grid.merge_down();

	}

	int mpGame::logic()
	{


		if(new_game.nscore != 0 && new_game.nclear != 0)
		{
			totals["score"] += new_game.nscore;
			totals["totald"] += new_game.nclear;
			new_game.nscore = 0;
			new_game.nclear = 0;
		}

		// default keyboard input

		Uint8 *keys = SDL_GetKeyState(0);
		static Uint32 start_time = SDL_GetTicks();

		Uint32 diff=SDL_GetTicks()-start_time;
		if(diff > 140)
		{
			start_time=SDL_GetTicks();
		} else return 0;


		if(keys[SDLK_LEFT])
			new_game.moveLeft();

		if(keys[SDLK_RIGHT])
			new_game.moveRight();

		if(keys[SDLK_UP])
			new_game.shiftColor();

		if(keys[SDLK_DOWN])
			new_game.moveDown();

#ifndef SYSTEM_PS3

		if(joy_on == true && mx::mxJoystick::joystickNum() > 0 && player1.isOpen())
		{

			if(player1.joystickGetButton(1))
				new_game.shiftColor();

			int value = player1.joystickGetAxis(0);

#ifdef __APPLE__

			if(value > 150)
				new_game.moveRight();

			if(value < 50)
				new_game.moveLeft();

			value = player1.joystickGetAxis(1);

			if(value > 150)
				new_game.moveDown();

#else
			if(value > 0)
				new_game.moveRight();

			if(value < 0)
				new_game.moveLeft();

			value = player1.joystickGetAxis(1);

			if(value > 0)
				new_game.moveDown();
#endif
		}
#else
		if(joy_on && mx::mxJoystick::joystickNum() > 0 && player1.isOpen())
		{

			if(player1.joystickGetAxis(10) > 150)
				new_game.moveDown();

			if(player1.joystickGetAxis(11) > 150)
				new_game.moveLeft();

			if(player1.joystickGetAxis(9) > 150)
				new_game.moveRight();


			if(player1.joystickGetButton(1))
			new_game.shiftColor();
		}

#endif


		if(new_game.is_gameOver() == true)
		{
			return 5;
		}


		return 0;
	}

	void mpGame::drawGrid(const int offset_x, const int offset_y)
	{

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
	}

	void mpGame::drawBlock(const gameBlock &current, const int offset_x, const int offset_y)
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

}



