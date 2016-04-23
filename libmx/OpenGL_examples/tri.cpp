#include<mx.h>




class openGL_triangle : public mx::mxWnd {


public:

	openGL_triangle(): mxWnd(640,480,0,0,true) {

		setTitle(" Drawing a triangle");

	}

	virtual void renderScreen() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);

		glBegin(GL_TRIANGLES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f,50.0f, 0.0f);
		glColor3f(0.0f,  1.0f, 0.0f);
		glVertex3f(-50.0f, 0.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(50.0f,  0.0f,   0.0f);


		glEnd();

		glFlush();

		SDL_GL_SwapBuffers();

	}

	virtual void eventPassed(SDL_Event &e) {
		mxWnd::eventPassed(e);
	}

	void init() {
		setMode(mx::ORTHO);
		resizeWindow(640, 480);
	}

	public: static int driver(int argc, char **argv) {

		openGL_triangle *tri = 0;
		try {

			tri =  new openGL_triangle();
			tri->init();
			tri->messageLoop();

			delete tri;
		}
		catch(...) {

			delete tri;
			std::cerr << "Error\n";
			return EXIT_FAILURE;

		}

		return EXIT_SUCCESS;

	}
};


STATIC_MAIN(openGL_triangle::driver)
