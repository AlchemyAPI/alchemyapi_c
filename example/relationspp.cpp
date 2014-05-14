#include "alchemy_api.hpp"
#include <string>
#include <iostream>

using namespace alchemy;

int main(int argc, char** argv)
{
	AlchemyFacade alchemyObj("data/api_key.txt");
	AlchemyAPI_RelationParams relationParams;

	relationParams.setMaxRetrieve(5);
	relationParams.setDisambiguate(true);
	relationParams.setSentiment(true);
	relationParams.setEntities(true);

	std::cout << alchemyObj.URLGetRelations("http://www.techcrunch.com", &relationParams);

	relationParams.setMaxRetrieve(1);

	std::cout << '\n' << std::endl;
	std::cout << alchemyObj.TextGetRelations("Madonna enjoys tasty Pepsi.  I love her style.", &relationParams);
}
