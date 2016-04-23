/* mxSocket test program:
		this program is intented to be run from the shell.
		not a windows program
*/

#include<mx.h>
#include<mxsocket.h>
#include<cstdlib>
#include<vector>

#define PORT 1024

int programLoop(int port);

int main(int argc, char **argv)
{

	try
	{

		return programLoop(PORT);
	} 
	catch( std::exception &e )
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch(...)
	{
		std::cerr << "unknown error \n";
		throw;
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}



template<typename Type, template<typename, typename Allocator = std::allocator<Type> > class container>
class  FunctionVector : public container<Type> {

public:
	void removeDead()
	{
		typename container<Type>::iterator i;

		for(i = this->begin();  i!=this->end(); i++)
		{
			if(i->isRunning() == false)
				this->erase(i);
		}
	}

};



struct User {
	std::string ip;
	mx::mxSocket socket;
};


class ListenObject : public mx::mxThread {

public:

	ListenObject(mx::mxSocket *socket) 
	 {  sock = socket;  }
	
	void listen(int port)
	{
		sock->listenAt(port);
		active = true;
		threadRun();
	}


	void sendText(std::string what)
	{

		size_t i;
		pmut.lockMutex();
		for(i=0;i<connected_sockets.size();i++)
		connected_sockets[i].socket.Write((char*)what.c_str(), what.length());
		pmut.unlockMutex();
	}


	virtual int threadExec() 
	{
		std::cout << "in thread \n";
		while(active)
		{
			
			User u;
			u.socket = mx::mxSocket ( sock->acceptNewSocket( u.ip ) );
			std::cout << "sockfd: " << u.socket.getsocket() << " new connection from: " << u.ip << "\n";
			pmut.lockMutex();
			connected_sockets.push_back(u);
			pmut.unlockMutex();

			mx::mxThreadObject<ListenObject, mx::mxSocket> user_thread(this, &ListenObject::handleConnection);
			user_thread.threadRun( u.socket );
			user_threads.push_back( user_thread );
			
		}
	
		return 0;
	}

	int handleConnection(mx::mxSocket socket)
	{

		std::cout << "socket thread started " << socket.getsocket() << "\n";
				char buffer[25];
		pmut.lockMutex();

		socket.Read((char*)buffer, 25);

		std::cout << "Hello World: " << buffer << "\n";

		// send it back
		socket.Write((char*)buffer,25);
		socket.closeSocket();		// close the socket

		pmut.unlockMutex();

		return 0;
	}
	
	const int connectedUsers() 
	{
		pmut.lockMutex();
		size_t s = connected_sockets.size();
		pmut.unlockMutex();
		return s;
	}


protected:
	mx::mxSocket *sock;
	mx::mxMutex  pmut;
	std::vector<User> connected_sockets;
	FunctionVector< mx::mxThreadObject<ListenObject, mx::mxSocket>, std::vector > user_threads;
	bool active;



};

int programLoop(int port)
{

	mx::mxSocket sock;
	
	sock.createSocket();

	ListenObject listobj(&sock);

	listobj.listen(PORT);

	int x = 0;
	while( 1 ) x++;

	return EXIT_SUCCESS;
}

