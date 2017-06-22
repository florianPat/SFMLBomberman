#include "Utils.h"

bool utils::isWordInLine(const std::string & word, const std::string & lineContent)
{
	size_t o = 0;
	bool result = false;
	while (o < lineContent.size() && !result)
	{
		o = lineContent.find(word[0], o);
		std::string searchWord(word);
		auto it = ++searchWord.begin();
		for (; o < lineContent.size(); ++it)
		{
			if (it != searchWord.end())
			{
				if (lineContent.at(++o) == it[0])
					continue;
				else
					break;
			}
			else
			{
				result = true;
				break;
			}
		}
	}

	return result;
}