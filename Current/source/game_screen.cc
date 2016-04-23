#include "game_screen.h"
#include<mxpng.h>

namespace current {

	gameScreen::gameScreen(mx::mxSurface &f) : front(f) {

	}

	gameScreen::~gameScreen() {

	}

	bool gameScreen::loadImage(mx::mxSurface &surf, const char *src) throw (mx::mxException<std::string>) {
		mx::mxPng p;
		p.pngOpen(src);
		SDL_Surface *surface = p.LoadPNG();
		p.pngClose();

		if(surface == 0)
			throw mx::mxException<std::string>(" could not load image " + std::string(src) + "\n");

		surf = surface;
		return true;

	}

}

