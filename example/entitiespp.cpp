#include "alchemy_api.hpp"
#include <string>
#include <iostream>
using namespace alchemy;

int main(int argc, char** argv)
{
	AlchemyFacade* alchemyObj = new AlchemyFacade("data/api_key.txt");
	AlchemyAPI_EntityParams* entityParams = new AlchemyAPI_EntityParams();
	
	entityParams->setMaxRetrieve(5);
	entityParams->setDisambiguate(true);
	entityParams->setSentiment(true);
	
	std::cout << alchemyObj->URLGetRankedNamedEntities("http://www.techcrunch.com", entityParams);

	entityParams->setMaxRetrieve(1);
	
	std::cout << "\n\n";
	std::cout << alchemyObj->TextGetRankedNamedEntities("Carolina Panthers receiver Steve Smith says he was just having fun and it was a freak accident when he broke his left arm playing flag football.", entityParams);

	delete entityParams;
	delete alchemyObj;
}
