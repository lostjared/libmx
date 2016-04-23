/*
 * libmx example - surface manipulation
 * 
 * locking/unlocking surfaces, ploting/getting pixels using mxPainter
 * copying surfaces to other surfaces
 * fliping the front surface to the screen
 * 
 * does a few cheap tricks to produce a nice visual effect
 * 
 */




#include<mx.h>
#include<mxpng.h>

class mx_Window : public mx::mxWnd {
	
public:
	mx_Window(std::string title="surface_test", std::string img="img/test.png") : mxWnd(640, 480, 0)
	{
		setTitle(title);
		mx::mxPng p;
		SDL_Surface *surf;
		if( p.pngOpen(img) ) {
			surf = p.LoadPNG();
			image = surf;
			p.pngClose();
		}
		
		motion = 1;
	
	}

	virtual void eventPassed(SDL_Event &e) {

		mxWnd::eventPassed(e);

		if(e.type == SDL_KEYDOWN)
		switch(e.key.keysym.sym) {
		case SDLK_UP:
			motion = 1;
			break;
		case SDLK_LEFT:
			motion = 2;
			break;
		case SDLK_RIGHT:
			motion = 3;
			break;
		default:
			break;
		}
	}
	
	virtual void renderScreen() {
		
		procSurface();
		front.copySurface(image, 0, 0);
		front.Flip();
	}
	
	
	void procSurface() {
		
		static mx::mxPainter p(image);
		register int i,z;
		
		p.lock();
		
		mx::Color col1, col2, col3, col4;
		
		
		for(i = 0; i < image.width() - 4; i++) {
			for(z = 0; z < image.height() - 4; z++) {
				
				col1 = p.getpixel(i,z);
				col2 = p.getpixel(i+1, z);
				col3 = p.getpixel(i+2, 2+z);
				col3 = p.getpixel(i, z+1);
				
				mx::Color finalCol;
				
				unsigned char col_r, col_g, col_b;
				
				col_r = ( col1.color.colors[0] + col2.color.colors[0] + col3.color.colors[0]  +  col4.color.colors[0] );
				col_g = ( col1.color.colors[1] + col2.color.colors[1] + col3.color.colors[1] + col4.color.colors[1] );
				col_b = ( col1.color.colors[2] + col2.color.colors[2] + col3.color.colors[2] + col4.color.colors[2]);
				
				switch(motion) {
				case 1: finalCol = mx::Color( col_r, col_g, col_b ); break;
				case 2: finalCol = mx::Color( col_g, col_b, col_r ); break;
				case 3: finalCol = mx::Color( col_b, col_g, col_r ); break;
				}
		
				finalCol.color.value /= 3;
				p.setpixel(i,z,mx::Color::mapRGB(image, finalCol));
			
			}
		}
		
		p.unlock();
		
		
	}
protected:
	mx::mxSurface image;
	int motion;
	
};


int main(int argc, char **argv) {

	int rt_code = EXIT_SUCCESS;
	
	try {
		
		mx_Window mx_win("Surface test");
		
		rt_code = mx_win.messageLoop();		
	}
	catch( mx::mxException<std::string> &e ) 
	{
		e.printError(std::cerr);
		return EXIT_FAILURE;	
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;	
	}
	catch(...)
	{
		std::cerr << "unknown exception, aborting..\n";
		return EXIT_FAILURE;
	}

	return rt_code;
}
