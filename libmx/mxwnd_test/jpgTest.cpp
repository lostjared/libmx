#include<mx.h>
#include<string>
#include<iostream>


class mxX : public mx::mxWnd {
	mx::mxImage img;
public:
	mxX() : mxWnd(640,480,0) {
		// load jpeg directly with mxJpeg
		mx::mxJpeg jpg;
		jpg.jpgOpen("test.jpg");
		SDL_Surface *surf = jpg.LoadJPG();
		assert ( surf != 0 );
		surface = surf;
		jpg.jpgClose();
		// load png,jpeg,bmp files with simplified loader

		SDL_Surface *surfz = img.loadIMG("test.jpg");
		assert ( surfz != 0 );
		std::cout << "Successful Load of Image: " << surfz->w << ":" << surfz->h << "\n";
		SDL_FreeSurface( surfz);

		if ( img.loadIMG(surface2, "test.jpg") == false )
			std::cout << "failed to load surface.\n";
	}

	virtual void renderScreen() {
		front.copySurface(surface, 0, 0);
		front.Flip();
	}

	virtual void eventPassed(SDL_Event &e) {
		switch(e.type) {
			case SDL_QUIT:
				quit();
				break;
			case SDL_KEYDOWN:

				if(e.key.keysym.sym == SDLK_RETURN) {
					img.saveJPG(front.getSurface(), "ss.jpg");
					img.saveJPG(surface.getSurface(), "ss2.jpg");

				}


				break;
		}
	}

	// fake main function
	static int main(int argc, char **argv) {
		using std::string;
		try {
			mxX wnd;
			return wnd.messageLoop();

		} catch( mx::mxException<string> &e ) {
			e.printError(std::cerr);
		}
		catch( std::exception &e ) {
			std::cerr << e.what() << "\n";
		}
		return 0;
	}
protected:
	mx::mxSurface surface, surface2;
};


STATIC_MAIN(mxX::main)




