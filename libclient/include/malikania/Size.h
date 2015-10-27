#ifndef _MALIKANIA_SIZE_H_
#define _MALIKANIA_SIZE_H_

namespace malikania {

class Size
{
public:
	int width{0};
	int height{0};
};

inline bool operator==(const Size &lhs, const Size &rhs)
{
	return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const Size &lhs, const Size &rhs)
{
	return !(lhs == rhs);
}

}// !malikania

#endif // !_MALIKANIA_SIZE_H_
