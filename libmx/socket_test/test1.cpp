/* mxSocket test program:
		this program is intented to be run from the shell.
		not a windows program
*/

#include<mx.h>
#include<mxsocket.h>




int main(int argc, char **argv)
{


	try
	{

		mx::mxSocket sock;
		sock.createSocket();
		std::cout << "connecting.....\n";
		sock.connectTo("205.234.202.125", 80);
		std::string str_get = "GET /test1.txt HTTP/1.0\n\n";
		sock.Write((char*)str_get.c_str(), str_get.length());
		static char buffer[1024];
		std::cout << "reading...\n";
		int result;
		while ( ( result = sock.Read((char*)buffer, sizeof(buffer)) ) != 0 )
		{
			buffer[result] = 0;
			std::cout << buffer;
		}
		sock.closeSocket();
		
	}
	catch( std::exception &e )
	{

		std::cerr << e.what() << "\n";
		
	}
	catch(...)
	{



	}

	return EXIT_FAILURE;
}

