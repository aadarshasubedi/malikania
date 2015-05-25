#ifndef _MALIKANIA_COMMAND_H_
#define _MALIKANIA_COMMAND_H_

#include <string>

namespace malikania {

class CommandInfo {
public:
	std::string name;
	std::string summary;
	std::string help;
};

class Command {
private:
	std::string m_name;
	std::string m_summary;
	std::string m_help;
};

} // !malikania

#endif // !_MALIKANIA_COMMAND_H_
