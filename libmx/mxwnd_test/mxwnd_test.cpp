/*
 * 		libmx example
 * 
 * 
 *      mxWnd test program
 * sets up a simple MX window and plots random pixels
 * 
 */


#include<mx.h>
#include<mx_exception.h>


class window : public mx::mxWnd {
public:
	window() : mxWnd(640, 480, 0) {
		setTitle("mxwnd test");	
	}
	
	virtual void eventPassed(SDL_Event &e) {
		
		mxWnd::eventPassed(e);	
	}
	
	virtual void renderScreen() {
		
		static mx::mxPainter paint(front);
		
		paint.lock();
		register int i,z;
		
		for(i = 0; i < front.width(); i++) 
			for(z = 0; z < front.height(); z++)
				paint.setpixel(i,z, rand()%0xFFFFFFFF);
		
		paint.unlock();
		
		front.Flip();
		
	}
};

int main(int argc, char **argv) {

	try {		
		
		window win;
		return win.messageLoop();
		
	}
	catch ( mx::mxException<std::string> &e ) {
		
		e.printError(std::cerr);
			
	}
	catch ( std::exception &e ) {
		
		std::cerr << e.what () << "\n";
		
	}
	catch (...) {
		
		std::cerr << "unknown exception.\n";
	}
	
	
	return EXIT_FAILURE;
}


