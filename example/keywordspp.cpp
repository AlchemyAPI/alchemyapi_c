#include "alchemy_api.hpp"
#include <string>
#include <iostream>
using namespace alchemy;

int main(int argc, char** argv)
{
	AlchemyFacade* alchemyObj = new AlchemyFacade("data/api_key.txt");
	AlchemyAPI_KeywordParams* keywordParams = new AlchemyAPI_KeywordParams();
	
	keywordParams->setMaxRetrieve(2);
	keywordParams->setKeywordExtractMode(AlchemyAPI_KeywordParams::STRICT);
	keywordParams->setShowSourceText(false);
	keywordParams->setSourceText(AlchemyAPI_KeywordParams::RAW);
	
	std::cout << alchemyObj->URLGetRankedKeywords("http://www.techcrunch.com", keywordParams);
	
	keywordParams->setMaxRetrieve(1);
	
	std::cout << "\n\n";
	std::cout << alchemyObj->TextGetRankedKeywords("Carolina Panthers receiver Steve Smith says he was just having fun and it was a freak accident when he broke his left arm playing flag football.", keywordParams);

	delete keywordParams;
	delete alchemyObj;
}
