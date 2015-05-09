#ifndef SIZE_H
#define SIZE_H

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

#endif // SIZE_H
