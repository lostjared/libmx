/*
 * 
 * 
 * 
 * This libmx example is one to stimulate ideas
 * there are quite a few different design methods used in this example
 * 
 * 
 * - jared bruni
 * 
 * 
 * 
 */










#include<mx.h>

using mx::mxWnd;

typedef unsigned int uint;

// some example classes , just to stimulate idea's of what you could create

class programObject {

public:
	programObject(mxWnd *p) : ptr(p) , position(0,0,0,0) {
			shade = 0;
	}
	
	void setPos(uint x, uint y, uint width, uint height) {
		position = mx::Rect(x,y,width,height);
	}
	void setPos(const mx::Rect &r) {
		position = mx::Rect(r);
	}
	
	void setColor(unsigned char r, unsigned char g, unsigned char b) {
		shade = mx::Color(r,g,b);		
	}
	
	void setColor(unsigned long x) {
		shade = x;
	}
	
	void setColor( const mx::Color &c ) {
		shade = mx::Color( c );		
	}
	
	mx::Rect &getPos() { return position; }
	mxWnd *ptr;
	mx::Rect position;
	mx::Color shade;
};

class movementObject : public programObject {
	
public:
	movementObject(mxWnd *p) : programObject(p) {	}
	void stretch_width(int w) { position.rsize.width += w; }
	void stretch_height(int h) { position.rsize.height += h; }
	void lower_width(int w)  { stretch_width(-w); }
	void lower_height(int h) { stretch_height(-h); }
	void moveLeft() { position.point.x--; }	
	void moveRight() { position.point.x++; }
	void moveUp() {  position.point.y--; }
	void moveDown() { position.point.y++; }
	bool checkBounds() {	
		if(position.point.x <= 0 || position.point.y <= 0 || position.point.x > ptr->renderSurface()->w || position.point.y > ptr->renderSurface()->h )
			return false;
		return true;
	}
	
protected:
	
};

// a Pix "Cell"

class Pixcell : public movementObject {
	
public:
	Pixcell(mxWnd *p) : movementObject(p) {
		
		birth();
		
	}
	
	void birth() { alive = true; }
	void death() { alive = false; }
	const bool is_alive() const { return alive; }
	void initPixcell(mx::Rect start_rect, mx::Color color) {

		position = mx::Rect(start_rect);
		shade = mx::Color(color);
	}
	
	inline void plotPixel(mx::mxPainter *p) {
		p->setpixel(position.x(), position.y(), mx::Color::mapRGB(*p->getSurface(), shade));	
	}
	
	void mergePixcell( Pixcell &p ) {
		
		shade.color.value += p.shade.color.value;
		p.death();
		
	}
	
protected:
	bool alive;
	
};


class pixcellEmiter {
	
	
public:
	pixcellEmiter(mxWnd *p) : ptr(p) {
		
	
		// fill vector
		
	}
	
	
	void drawPix(mx::mxPainter *painter) {
		
		// painter->setpixel etc..
		
		
	}
	
protected:
	std::vector<Pixcell> p;
	mxWnd *ptr;
};


class mx_Window : public mx::mxWnd {
	
	
public:
	mx_Window() : mxWnd(640,480,0), emiter(this) {
	}
	
	virtual void eventPassed(SDL_Event &e) {
		mxWnd::eventPassed(e);	
		// use objects in this class		
	}
	
	virtual void renderScreen() {
		
		static mx::mxPainter paint(front);
		
		paint.lock();

		emiter.drawPix(&paint); // passing pointer of mxPainter
		
		paint.unlock();
		
		front.Flip();
		
	}
	
protected:

	pixcellEmiter emiter;
};


// driver 
int main(int argc, char **argv) {
	
	
	try {
		mx_Window window1;
		return window1.messageLoop();
	} catch ( mx::mxException<std::string> & e ) {
		e.printError(std::cerr);
	}
	
	return 0;
}










