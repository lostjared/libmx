#ifdef HAVE_CONFIG_H
#include<config.h>
#endif


#include "mxmp.h"
#include<mxvector.h>
#include<cstdlib>
#include<ctime>


int main(int argc, char **argv)
{
	try
	{

		srand(static_cast<unsigned int>(time(0)));

		mx::mxArgs argz(argc, argv);
		std::string path = argz.testForItem("--path");
		std::string flag = argz.testForItem("--full");
		std::string width = argz.testForItem("--width");
		std::string height = argz.testForItem("--height");
		
		unsigned int flags = 0;
		if(flag == "true") flags = SDL_FULLSCREEN;
		if(path != "") chdir(path.c_str());

#ifdef DARWIN		
		if(argv[0][0] == '/') {
			
				std::string p = argv[0];
			std::string path = p.substr(0, p.rfind("/"));
			path += "/../Resources";
			std::cout << "setting path to: " << path << "\n";
			chdir(path.c_str());
		}
#endif
		
		
		if(width == "" | height == "") width="720", height="480";
		
		mp::mxmpWnd wnd(path,flags, atoi(width.c_str()), atoi(height.c_str()));	

		
		return wnd.messageLoop();

	}
	catch ( mx::mxException<std::string> &e )
	{
		e.printError(std::cerr);
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << "\n";
	}
	catch ( ... )
	{
		std::cerr << "unknown exception, aborting..\n";
	}

	return EXIT_FAILURE;
}


