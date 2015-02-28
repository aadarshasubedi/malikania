#include <iostream>

#include <malikania/ServerApplication.h>
#include <malikania/ServerDirectoryLoader.h>

using namespace malikania;

int main(int argc, char **argv)
{
	try {
		ServerApplication app(argc, argv);
		ServerDirectoryLoader loader("/home/markand/mygame");

		app.run(std::move(loader));
	} catch (const std::exception &ex) {
		std::cerr << ex.what() << std::endl;
		std::exit(1);
	}

	return 0;
}
