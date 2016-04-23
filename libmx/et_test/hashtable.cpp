#include<mx_exception.h>
#include<mxtree.h>
#include<iostream>
#include<string>
#include<cstdlib>

#define SIZE_MAP 1500

typedef struct {

	int var_value;
	std::string vname;

} TableItem;


std::ostream &operator<<(std::ostream &out, TableItem &t)
{
	out << "Hash : " << t.vname << " value = " << t.var_value;
	return out;
}

int main(int argc, char **argv)
{

	unsigned int keys[10];
	unsigned int index = 0;

	try
	{
		mx::mxList< unsigned int, mx::mxList < std::string, TableItem> > hash;

		char c[2];
		c[0] = 'Y';

		while( c[0] == 'Y' && index < 9)
		{
			std::cout << "enter hash key: ";
			char key[25];
			std::cin.getline(key, 25);
			char num[25];
			std::cout << "enter value: ";
			std::cin.getline(num, 25);
			unsigned int hash_key = mx::HashKey<std::string>(key,SIZE_MAP);
			keys[index++] = hash_key;

			hash[hash_key][key].var_value = atoi(num);
			hash[hash_key][key].vname = key;
			std::cout << index << "/10  added another Y/N: ";
			std::cin.getline(c, 2);
		}

		std::cout << "VALUES STORED IN TABLE: \n";

		for(unsigned int i = 0; i < index; i++)
		{
			hash[keys[i]].printNode();
		}
	}
	catch( mx::mxException<std::string> &e )
	{
		e.printError(std::cerr);
		return EXIT_FAILURE;
	}
	catch( std::exception &e )
	{
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "unhandled exception\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

