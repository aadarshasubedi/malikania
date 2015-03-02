#ifndef _MALIKANIA_APPLICATION_H_
#define _MALIKANIA_APPLICATION_H_

namespace malikania {

class Application {
public:
	Application(int argc, char **argv);
	virtual ~Application() = default;
};

} // !malikania

#endif // !_MALIKANIA_APPLICATION_H_

