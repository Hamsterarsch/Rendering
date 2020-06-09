#include "AssetNameFilter.hpp"
#include <unordered_set>


namespace App::Ui::Filter
{
	static const std::unordered_set<char> forbiddenCharacters{ '/', '\\', '.', ' ' };

	
	bool AssetNameFilter::CharacterIsForbidden(char character)
	{
		return forbiddenCharacters.find(character) != forbiddenCharacters.end();
		
	}

	
}
