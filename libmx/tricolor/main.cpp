/*

	a Hazy Day Dream

		jared@lostsidedead.biz

*/


#ifdef HAVE_CONFIG_H
#include<config.h>
#endif



#include<mx.h>
#include<sstream>
#ifdef TEXT_ENABLED
#include<fstream>
#include<mx_tok.h>
#include<vector>
#include<mx_font.h>
#endif




extern "C" int swizzle(void *buffer, unsigned int size);


class demo : public mx::mxWnd  {

private:

	color::M4Color3D<250,250> background;
	SDL_Surface *surface;

	bool go;

public:

	demo(Uint32 flags) : mxWnd(640,480, flags)	{
		surface = SDL_LoadBMP("background.bmp");

		if(surface == 0) throw mx::mxException<std::string>(" could not load default bitmap program halted\n ");

		fill_background(surface);
		//background.randbuffer();
		background.triblend();
		go = false;

		SDL_WM_SetCaption(" Press Enter to start the Trip ", 0);	

#ifdef TEXT_ENABLED

	std::fstream file;

	file.open("lines.txt", std::ios::in | std::ios::binary);
	if(!file.is_open())
	{
		throw mx::mxException<std::string>( " missing file lines.txt check repoistory this build has sub messages enabled \n");

	}

	size_t len;
	file.seekg(0, std::ios::end);
	len = file.tellg();
	file.seekg(0, std::ios::beg);
	char *buf = new char [len+1];
	file.read((char*)buf, len);
	buf[len] = 0;
	file.close();
	std::string text (buf);
	delete [] buf;

	token::tokenize<std::string>(text, std::string("\n"), sayings);
	srand(static_cast<unsigned int>(time(0)));

	sys_font.loadFont("system.mxf");

	srand(static_cast<unsigned int>(time(0)));

#endif


	}

	~demo() // no-throw guarantee 
	{

		if(surface != 0)
			SDL_FreeSurface(surface);

	}

	void input_file(std::string str)
	{
		SDL_Surface *surf = SDL_LoadBMP(str.c_str());
		if(surf == 0) std::cout << "error could not load " << str << " using default\n";
		else
		{

			SDL_FreeSurface(surface);
			surface = surf;

		}

	}

	void eventPassed(SDL_Event &e)
	{

		switch(e.type)
		{
		case SDL_QUIT:
		quit();
		break;
		case SDL_KEYDOWN:
		{

			switch(e.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			quit();
			break;
			case SDLK_RETURN: // enter haze
			{
				color::Color3D col;
				col.setcolor(25.42f, 21.44f,-144.0f,0);
				background.addColor3D(col);
				go = true;
			}
			break;
			case SDLK_SPACE: // take snapshot
			{
				std::ostringstream snap;
				static int snap_count = 0;

				snap << "snapshot" << ++snap_count << ".bmp";
				SDL_SaveBMP(front, snap.str().c_str());
				SDL_Delay(25); // small delay
			}
			break;

			default:
				
			break;
			}

		}
		break;


		}

	}

	int  GetFX(int curw,int x, int nw) {
        	float xp = (float)x * (float)curw / (float)nw;		
        	return (int)xp;
	}

	int  GetFY(int curh, int y, int nh) {
        	float yp = (float)y * (float)curh / (float)nh;
        	return (int)yp;
	}



	void CopyToSurface(SDL_Surface *surface)
	{

		if(SDL_MUSTLOCK(surface))
		{

			SDL_LockSurface(surface);

		}


		void *buffer = surface->pixels;
		unsigned int *ptr = (unsigned int *)buffer;
		

	
		int i,z;
		for(i=0; (i < static_cast<int>(surface->w)) && (i < static_cast<int>(background.w)); i++)
		{
			for(z = 0; (z < static_cast<int>(surface->h)) && (z < static_cast<int>(background.h)) ; z++)
			{

				unsigned int num = background.row[i][z].toInteger();

				if(num == 0)
				{



				}
				else
				
				{

					ptr[i+(z*surface->w)] = num;
							
	
				}

			}
		}


		


		if(SDL_MUSTLOCK(surface))
		{

			SDL_UnlockSurface(surface);
		}



	}

	void AlphaCopyToSurface(SDL_Surface *surface, SDL_Surface *alpha)
	{

		static float alphav = 1.0f;

		alphav += 0.1f;


		if(SDL_MUSTLOCK(surface))
		{

			SDL_LockSurface(surface);

		}

		if(SDL_MUSTLOCK(alpha))
		{

			SDL_LockSurface(alpha);

		}


		void *buffer = surface->pixels;
		void *abuffer = alpha->pixels;
		unsigned int *ptr = (unsigned int *)buffer;
	
		int i,z;
		for(i=0; (i < surface->w); i++)
		{
			for(z = 0; (z < surface->h); z++)
			{

				int ti=0,tz=0;
				ti=GetFX(background.w, i, surface->w);
				tz=GetFY(background.h, z, surface->h);

				unsigned int num = background.row[ti][tz].toInteger();


				mx::_Color col1;
				col1.value = num;

				if(num == 0)
				{



				}
				else
				
				{

					mx::_Color col2;

					static float alpha = 0.0f;

					unsigned char *pbuf = (unsigned char*)abuffer;
					unsigned char *ebuf = (unsigned char*)buffer;



					pbuf += (z*surface->pitch)+i; // yeah I get it still looked cool tho
					ebuf += (z*surface->pitch)+i; // 
					


					col2.colors[0] += static_cast<unsigned char>(alpha*pbuf[0]);
					col2.colors[1] += static_cast<unsigned char>(alpha*pbuf[1]);
					col2.colors[2] += static_cast<unsigned char>(alpha*pbuf[2]);

					alpha += 0.00000000000000f;	

					mx::_Color col3;
					col3.colors[0] = static_cast<unsigned char>(col1.colors[0] + (1-alphav)*col2.colors[0]);
					col3.colors[1] = static_cast<unsigned char>(col1.colors[1] + (1-alphav)*col2.colors[1]);
					col3.colors[2] = static_cast<unsigned char>(col1.colors[2] + (1-alphav)*col2.colors[2]);
					num = col3.value;					
					ptr[i+(z*surface->w)] = num;
							
	
				}

			}
		}


		
		swizzle((void*)alpha->pixels, ((alpha->w*alpha->h)*alpha->format->BytesPerPixel)-alpha->pitch);



		if(SDL_MUSTLOCK(surface))
		{

			SDL_UnlockSurface(surface);
		}

		if(SDL_MUSTLOCK(alpha))
		{

			SDL_UnlockSurface(alpha);

		}


	}

	void fill_background(SDL_Surface *surface)
	{

		if(SDL_MUSTLOCK(surface))
		{

				if(SDL_LockSurface(surface) < 0)
					return;

		}


		void *buffer = surface->pixels;

		int i = 0, z = 0;

		for(i = 0; (i < static_cast<int>(surface->w)) && ( i < static_cast<int>(background.w)); i++)
		{
			for(z = 0; (z < static_cast<int>(surface->h)) && (z < static_cast<int>(background.h)) ; z++)
			{


				unsigned char *ptr = (unsigned char *)buffer;
				ptr += (z*surface->pitch)+(i*3);

				unsigned char r,g,b,a;

				r = ptr[0];
				g = ptr[1];
				b = ptr[2];
				a = ptr[3];
		
				color::Color3D col;

				col.setcolor(static_cast<float>(r), static_cast<float>(g), static_cast<float>(b), static_cast<float>(a));
				background.setp(i,z,col);

			}

		} 






		if(SDL_MUSTLOCK(surface))
		{
			SDL_UnlockSurface(surface);
		}



	}

	void renderScreen()
	{


		


		if(go == false) background.increase(2.5f, -1.0f, 0.0f);
		else
		{

			static float alpha_value = 1.0f;

			background.increase(4.0f*alpha_value, 1.0f*alpha_value, -10.0f*alpha_value);

			alpha_value += 0.1f;

		}


#ifdef TEXT_ENABLED

		int cur_x = 0 , cur_y = 0;
		// print text	
		std::string t;
		t = grabText();

		do
		{

			cur_x = rand()%front.getSurface()->w-100;
			cur_y = rand()%front.getSurface()->h-100;

		} while ( cur_x < 0 || cur_y < 0 );

		sys_font.printText(front, cur_x, cur_y, SDL_MapRGB(front.getSurface()->format, 255, 255, 255), t);
#endif

		AlphaCopyToSurface(front, surface);


#ifdef TEXT_ENABLED
		// print text	
		sys_font.printText(front, cur_x, cur_y,SDL_MapRGB(front.getSurface()->format, 255, 255, 255), t);
#endif

	

		
		// surface looks kinda neat too
		
		front.Flip();

	}

#ifdef TEXT_ENABLED
	std::string  grabText()
	{

		std::string text;
		int pos;
		do {
			pos = rand()%sayings.size()-2;

		} while ( pos < 0 );


		text = sayings[pos];

		if(text.length() > 10) text = text.substr(0, 10);
		return text;
	}


	std::vector<std::string> sayings;
	mx::mxRegularFont sys_font;

#endif



};













int main(int argc, char **argv)
{


	try
	{
		mx::mxArgs argx(argc, argv);

		std::string file = argx.testForItem("--input");
		std::string full = argx.testForItem("--full");


		Uint32 flags = 0;

		if(full == "true") flags = SDL_FULLSCREEN;
	
		demo d(flags);

		if(file != "") d.input_file(file);

		return d.messageLoop();

	}
	catch (mx::mxException<std::string> &e)
	{

		e.printError(std::cout);
	
	}
	catch (std::exception &e)
	{

		std::cout << e.what() << "\n";

	}
	catch(...)
	{

		std::cout << "Unknown\n";
	}

	return EXIT_FAILURE;
}



