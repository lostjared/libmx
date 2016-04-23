/*

	reworded recoded static fuzz v1
					-jared@lostsidedead.biz


	http://lostsidedead.com/svn


*/


#include<mx.h>
#include"mxvector.h"
#include<mxpixel.h>
#include<sstream>
#include<mx_font.h>
#include<mxeternal.h>



class fuzzyMainWindow : public mx::mxWnd {


protected:
	bool pressed;
	int amount;
	SDL_Surface *bmp;

public:


	virtual  ~fuzzyMainWindow() { 
		
	}

	fuzzyMainWindow(std::string file_name, std::string full) : mxWnd(640, 480, (full == "true") ? SDL_FULLSCREEN : 0, SDL_LoadBMP("icon.bmp"))
	{

		setTitle("LostSideDead - [ index of static fuzz  ]");

		sys_font.loadFont("system.mxf");

		enableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
		
		pressed = false;
		bmp = SDL_LoadBMP(file_name.c_str());	

		if(!bmp) throw mx::mxException<std::string>(" Error could not find default background.bmp ");		

		front_surface.setCopySurface(bmp);
		
		data.initDataFromSurface(front_surface);	
		amount = 1;

	}

	virtual void eventPassed(SDL_Event &e)
	{

		switch(e.type)
		{
		case SDL_QUIT:
			quit();
			break;
		case SDL_KEYDOWN:

			if(e.key.keysym.sym == SDLK_ESCAPE) quit();
			if(e.key.keysym.sym == SDLK_LEFT)   if(amount>1) amount--;
			if(e.key.keysym.sym == SDLK_RIGHT)  if(amount<100) amount++;
			
			
			break;
		}

	}

	virtual void renderScreen()
	{

		data.copyDataToSurface(front_surface, amount);
		front.copySurface(front_surface, 0, 0);

		static SDL_Rect rc = { 0, 0, 640, 25 };
		static Uint32 col_black = SDL_MapRGB(front, 0, 0, 0);
		static Uint32 col_white = SDL_MapRGB(front, 255, 255, 255);
		
		std::ostringstream stream;
		
		SDL_FillRect(front, &rc, col_black);
		
		stream << "Fuzz Index: " << amount;
		
		sys_font.printText(front, 5, 5, col_white, stream.str());

		front.Flip();
	}

protected:
	mx::mxPixelData data;
	mx::mxSurface front_surface;
	mx::mxRegularFont sys_font;
	

};


int main(int argc, char **argv)
{


	try
	{

		mx::mxArgs argz(argc, argv);
		std::string input=argz.testForItem("--input");
		std::string full=argz.testForItem("--full");

		if(input=="")
		input="background.bmp";

		fuzzyMainWindow w(input, full);
		return w.messageLoop(); // EXIT_SUCCESS

	}
	catch( mx::mxException<std::string> &e )
	{

		e.printError(std::cerr);

	}
	catch (std::exception &e)
	{

		std::cerr << e.what() << "\n";

	}
	catch ( ... )
	{
		std::cerr << "Error unknown exception\n";
	
	}

	return EXIT_FAILURE;

}




