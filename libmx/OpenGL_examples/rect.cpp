// Quad and Fog Demo

#include<mx.h>

class openGL_triangle : public mx::mxWnd {


public:

	openGL_triangle(): mxWnd(640,480,0,0,true) {

		setTitle(" Drawing a 3D Rectangle and Fog Example");

	}

	virtual void renderScreen() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);

		static float rotate = 0.0f;

		glTranslatef(0.0f, 0.0f,rotate);

		rotate += 0.01f;

		if(rotate > 50) rotate = 0;

		glBegin(GL_QUADS);

		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-50.0f, 50.0f, 1.0f);
		glColor3f(0.0f, 0.0f,1.0f);
		glVertex3f(50.0f, 50.0f,  1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(50.0f, 0.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f, 0.0f, 1.0f);

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
		glEnable(GL_FOG);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		GLfloat fog_light[] = { 0.5f, 0.5f, 0.5f,  1.0f };
		glFogfv(GL_FOG_COLOR, fog_light);
		glFogf(GL_FOG_START, 5.0f);
		glFogf(GL_FOG_END, 30.0f);
		glFogi(GL_FOG_MODE, GL_LINEAR);

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
