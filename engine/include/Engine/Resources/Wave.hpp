#pragma once
#include "Sound.hpp"

class Wave : public Sound
{
public:

	Wave() = default;
	Wave(const char* filepath);
	~Wave() = default;
};