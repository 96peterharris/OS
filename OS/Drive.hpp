#pragma once
#include <array>
class Drive
{
public:
	const static int drivesize = 1024;
	const static int sizeofblock = 32;
	const static int blocks = Drive::drivesize / Drive::sizeofblock;
	std::array <char, drivesize> drivespace{ 0 };
};
