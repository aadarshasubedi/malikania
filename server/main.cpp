#include <malikania/ServerApplication.h>
#include <malikania/ServerDirectoryLoader.h>

using namespace malikania;

int main(int argc, char **argv)
{
	ServerApplication app(argc, argv);
	ServerDirectoryLoader loader("/home/markand/mygame");

	app.run(std::move(loader));

	return 0;
}
