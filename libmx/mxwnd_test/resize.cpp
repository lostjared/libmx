/* libmx-0.4 example using screen stretching */

#include<mx.h>
#include<args.h>

const int size_w = 1280;
const int size_h = 1024;

class mxTube : public mx::mxWnd {
	
public:
	mxTube(int width, int height) : mxWnd(mx::Size(width, height), mx::Size(size_w, size_h))  {}
	
	virtual void eventPassed(SDL_Event &e) {
		
		mxWnd::eventPassed(e);		
	}
	
	virtual void renderScreen() {
	
		static mx::mxPainter p(sizedSurface());
		p.fillRect(mx::Rect(0, 0, size_w , size_h ), mx::Color(rand()%255, rand()%255, rand()%255));
		sizedFlip();
	}
	
};


int main(int argc, char **argv) {
	
	try {
		
		int tX = 640, tY = 480;	
		mx::mxArgs argz(argc, argv);
		
		std::string str_width = argz.testForItem("--width");
		std::string str_height = argz.testForItem("--height");
		
		if(str_width != "" && str_height != "") {
			tX = atoi(str_width.c_str());
			tY = atoi(str_height.c_str());			
		}
		
		mxTube tube(tX,tY);;
		return tube.messageLoop();
	}
	catch ( mx::mxException<std::string> &e ) {
		e.printError(std::cerr);
	} catch( std::exception &e ) {
		std::cerr << e.what() << "\n";
	} catch(...) {
		std::cerr << "unknown..\n";
	}
	
	return 0;
	
}



