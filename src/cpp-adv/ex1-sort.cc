#include <ostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>

void read(std :: istream& in, std :: vector<std :: string>& text)
{
	std :: string line;
	while (std :: getline(in, line))
		text.push_back(line);
}

int main(int argc, char* argv[])
{
	std :: vector<std :: string> text;
	if (argc < 2)
		read(std :: cin, text);
	else
	{
		std :: ifstream in(argv[1]);;
		if (not in)
		{
			std :: perror(argv[1]);
			return EXIT_FAILURE;
		}
		read(in, text);
	}

	std :: sort(text.begin(), text.end());

	std :: cout << std :: endl;
	std :: copy(text.begin(), text.end(),
			std :: ostream_iterator<std :: string>(std :: cout, "\n"));
}
