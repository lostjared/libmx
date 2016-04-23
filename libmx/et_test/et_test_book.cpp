#include<mxeternal.h>
#include<cstdlib>


typedef struct _bookDb {
	char book_id[25];
	char author[35];
	char booktitle[256];
} bookdb_Item;


void printHeader()
{

	std::cout << "Book eternal varaible example\nMenu--------\n1) add item\n2) search\n3) remove item\n4) quit\n5) display list\n"; 

}

std::ostream &operator<<(std::ostream &out, bookdb_Item &db) // overload  operator<< to handle structure
{
	out  << db.book_id << ":"  << db.booktitle << " by " << db.author;
}


int main(int argc, char **argv)
{

	try
	{
		mx::mxEternal<bookdb_Item> books("book.mxdb");

		std::cout << "Initial DB: " <<  books << "\n";

		while(1)
		{
			printHeader();
			char c[2];
			std::cin.getline(c,2);

			switch(c[0])
			{
			case '1':
			{
				char b_id[25];
				std::cout << "enter book id: ";
				std::cin.getline(b_id, 25);
				std::cout << "enter author: ";
				std::cin.getline(books[b_id].author, 25);
				std::cout << "enter book title: ";
				std::cin.getline(books[b_id].booktitle, 255);			
				sprintf(books[b_id].book_id, "%s", b_id);
				
			}
				break;
			case '2':
			{

				char b_id[25];
				std::cout << "enter book id: ";
				std::cin.getline(b_id, 25);

				if(books->find(b_id) != books->end())
				{
					std::cout << "Successfully found " << b_id << " in database.\n";
					std::cout << books[b_id].author << ":" << books[b_id].booktitle << "\n";

				} else std::cout << " could not find id in database\n";


			}
				break;
			case '3':
			{

				char b_id[25];
				std::cout << "enter book id: ";
				std::cin.getline(b_id, 25);
				mx::mxEternal<bookdb_Item>::iterator zi;
			
				zi = books->find(b_id);
				if(zi == books->end()) { std::cout << " could not find, cannot remove.\n"; continue; }
				
				books->erase(zi);

				std::cout << "erased item: " << b_id << "\n";

			}
				break;
			case '4':
				return EXIT_SUCCESS;
				break;
			case '5':
			{
				mx::mxEternal<bookdb_Item>::iterator zi;

				std::cout << "Book Database (" << books.getName() << ")\n";

				for(zi = books->begin(); zi != books->end(); zi++)
				{
					std::cout << "book id[" << zi->first << "] : ";
					std::cout << zi->second.booktitle << " author: " << zi->second.author << "\n";
				}

			}
				break;
			default:
				continue;
			}

		}

		return EXIT_SUCCESS;
	}
	catch(std::exception &err)
	{
		std::cerr << err.what() << "\n";
	}
	catch(...)
	{
		std::cerr << "unknown exception\n";
	}

	return EXIT_FAILURE;
}

