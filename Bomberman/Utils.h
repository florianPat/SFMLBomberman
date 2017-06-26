#pragma once

#include <string>
#include <vector>
#include "TextureAtlas.h"
#include "Animation.h"

namespace utils
{
	bool isWordInLine(const std::string & word, const std::string & lineContent);
	void addAnimation(std::vector<std::string> regionNames, std::string animationName, TextureAtlas& atlas, std::map<std::string, Animation>& animations);
}