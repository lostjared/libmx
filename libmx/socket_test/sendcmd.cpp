/* mxSocket test program:
		this program is intented to be run from the shell.
		not a windows program
*/


#include<mx.h>
#include<mxsocket.h>
#include<args.h>


int main(int argc, char **argv)
{

	try
	{
		char ipaddress[25];
		char port_num[10];
		char message[35];

		mx::mxSocket sock;
		sock.createSocket();
		std::cout << "connecting.....\n";

		


		std::cout << "enter ip: ";
		
		std::cin.getline(ipaddress, 25);

		std::cout << "port: ";
		std::cin.getline(port_num, 10);
		std::cout << "message: ";
		std::cin.getline(message,35);


		sock.connectTo(ipaddress, atoi(port_num));
		
		sock.Write((char*)message, strlen(message));
		static char buffer[1024];

	

		std::cout << "press enter to read";
		std::string c;
		std::cin >> c;

		sock.Read((char*)buffer, 1024);
		std::cout << buffer <<"\n";

		sock.closeSocket();
	} 
	catch (...)
	{

		std::cerr << "unknown Exception\n";
		return EXIT_FAILURE;

	}



	return EXIT_SUCCESS;
}

