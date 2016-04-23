/*
	Eternal Template Type Example
				-  Jared



		this example demonstrates the use of a persistant pool of varaibles (map)
	that autosaves to the disk on the programs termination and reloads on the
	start of execution. This simple command line application demonstrates the
	use of this concept by storing a map of integers to a file and keeping it up
	to date as the program starts and stops.

		also demonstrated in this example is how to over-ride the applications, 
	read/write for each type. string_Read, and string_write, have the class use the 
	standard library string class. Write your own read/write functions for unique 
	datatypes to create a type defined database.

*/

#include<mxeternal.h>
#include<cctype>
#include<cstdlib>
#include<fstream>
#include<mxtree.h>


using std::fstream;


int string_Write(fstream &fobj, std::string &strobject)
{
	unsigned int len = strobject.length();
	fobj.write((char*)&len, sizeof(len));
	fobj.write((char*)strobject.c_str(), len);
	return 0;
}

int string_Read(fstream &fobj, std::string &strobject)
{
	unsigned int len = 0;
	fobj.read((char*)&len, sizeof(len));
	char *tmp = new char [ len + 1 ];
	fobj.read((char*)tmp, len);
	tmp[len] = 0;
	strobject = tmp;
	delete [] tmp;
	return 0;
} 


int main(int argc, char **argv)
{



	try
	{

		mx::mxEternal<int> num("numeric.mxdb"); // create a eternal map called num saved to database file mxdb
		mx::mxEternal<std::string> svar("strings.mxdb",  new mx::mxEternalRW<std::string>( string_Read, string_Write ) );

		std::cout << num << "\n"; // using overloaded << operator output the map to the stream
		std::cout << svar << "\n";

		while(1)
		{
			std::string id; // map identifier (key)
			int value; // value

			std::cout << "Pool of variables, enter identifier: ";
			std::cin >> id; 
			svar[id] = id;
			std::cout << "enter its numeric value to be stored: ";
			std::cin >> value;
		
			std::cout << id << "/" << value << " is this correct if so press Y to enter into variable, Q to quit: ";
			char c;
			std::cin >> c;

			if(toupper(c) ==  'Y')
				num[id] = value; // set the value by identifier

			if(toupper(c) == 'Q') break; // quit the loop

			std::cout << "listing of variables:\n" << num << "\n"; // list the variable 
			
			std::cout << "listing of used strings: \n" << svar << "\n";
		}


		return EXIT_SUCCESS;

	} 
	catch(std::exception &e) // exception handling for standard exceptions 
	{
		std::cerr << e.what() << "\n";
	}
	catch(...)
	{
		std::cerr << "unknown exception\n";
	}

	return EXIT_FAILURE;
}

