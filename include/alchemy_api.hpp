/*************************************************************************
 * Summary: AlchemyAPI C++ Wrapper
 * Copyright: (C) 2014 AlchemyAPI, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMY_API_HPP__
#define __ALCHEMY_API_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"
#include "AlchemyAPI_EntityParams.hpp"
#include "AlchemyAPI_RelationParams.hpp"
#include "AlchemyAPI_KeywordParams.hpp"
#include "AlchemyAPI_CategoryParams.hpp"
#include "AlchemyAPI_ConceptParams.hpp"
#include "AlchemyAPI_LanguageParams.hpp"
#include "AlchemyAPI_TextParams.hpp"
#include "AlchemyAPI_ConstraintQueryParams.hpp"
#include "AlchemyAPI_TargetedSentimentParams.hpp"
#include "AlchemyAPI_TaxonomyParams.hpp"
#include "AlchemyAPI_ImageParams.hpp"
#include "AlchemyAPI_CombinedDataParams.hpp"
#include "AlchemyAPI_BaseParams.hpp"


namespace alchemy
{

class AlchemyFacade
{
public:
    AlchemyFacade(const std::string& filename)
    throw (std::runtime_error, std::invalid_argument)
    {
        apiObj.key_len = 0u;
        apiObj.key = NULL;
        apiObj.host_len  = 0u;
        apiObj.host= NULL;

        LoadAPIKey(filename);

        if (ALCHEMY_OK != alchemy_global_init())
        {
            alchemy_free_api_obj_util(&apiObj);
            throw std::runtime_error("Unable to initialize Alchemy API");
        }
    }

    ~AlchemyFacade()
    {
        alchemy_free_api_obj_util(&apiObj);
        alchemy_global_cleanup();
    }

    void SetAPIKey(const std::string& keyText)
        throw (std::runtime_error, std::invalid_argument)
    {
        alchemy_free_key_util(&apiObj);
        CheckRetCode(alchemy_set_key_util(&apiObj, keyText.c_str()));
    }

    void LoadAPIKey(const std::string& filename)
        throw (std::runtime_error, std::invalid_argument)
    {
        alchemy_free_key_util(&apiObj);
        CheckRetCode(alchemy_load_key_util(&apiObj, filename.c_str()));
    }

    void SetAPIHost(const std::string& hostText)
        throw (std::runtime_error, std::invalid_argument)
    {
        alchemy_free_host_util(&apiObj);
        CheckRetCode(alchemy_set_api_host(&apiObj, hostText.c_str()));
    }

    const std::string& URLGetAuthor(const std::string& url)
	throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_author, url);
    }

    const std::string& URLGetAuthor(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
	CheckRetCode(alchemy_url_get_author_params(
		&apiObj, url.c_str(),
		AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& HTMLGetAuthor(const std::string& html, const std::string& url)
	throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_author, html, url);
    }

    const std::string& HTMLGetAuthor(const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
	CheckRetCode(alchemy_html_get_author_params(
		&apiObj,  html.c_str(), url.c_str(),
		AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    
    const std::string& URLGetRankedNamedEntities(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_ranked_named_entities, url);
    }
    const std::string& URLGetRankedNamedEntities(const std::string& url, AlchemyAPI_EntityParams* entityParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_ranked_named_entities_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, entityParams->getParameters()));

        return buffer;

    }

    const std::string& HTMLGetRankedNamedEntities(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_ranked_named_entities, html, url);
    }
    const std::string& HTMLGetRankedNamedEntities(
        const std::string& html, const std::string& url, AlchemyAPI_EntityParams* entityParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_ranked_named_entities_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, entityParams->getParameters()));

        return buffer;
    }

    const std::string& TextGetRankedNamedEntities(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_ranked_named_entities, text);
    }
    const std::string& TextGetRankedNamedEntities(const std::string& text, AlchemyAPI_EntityParams* entityParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_text_get_ranked_named_entities_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, entityParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetRelations(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_relations, url);
    }
    const std::string& URLGetRelations(const std::string& url, AlchemyAPI_RelationParams* relationParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_relations_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, relationParams->getParameters()));

        return buffer;
    }

    const std::string& HTMLGetRelations(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_relations, html, url);
    }
    const std::string& HTMLGetRelations(
        const std::string& html, const std::string& url, AlchemyAPI_RelationParams* relationParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_relations_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, relationParams->getParameters()));

        return buffer;
    }

    const std::string& TextGetRelations(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_relations, text);
    }
    const std::string& TextGetRelations(const std::string& text, AlchemyAPI_RelationParams* relationParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_text_get_relations_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, relationParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetRankedConcepts(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_ranked_concepts, url);
    }
    const std::string& URLGetRankedConcepts(const std::string& url, AlchemyAPI_ConceptParams* conceptParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_ranked_concepts_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, conceptParams->getParameters()));

        return buffer;
    }

    const std::string& HTMLGetRankedConcepts(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_ranked_concepts, html, url);
    }
    const std::string& HTMLGetRankedConcepts(
        const std::string& html, const std::string& url, AlchemyAPI_ConceptParams* conceptParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_ranked_concepts_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, conceptParams->getParameters()));

        return buffer;
    }

    const std::string& TextGetRankedConcepts(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_ranked_concepts, text);
    }
    const std::string& TextGetRankedConcepts(const std::string& text, AlchemyAPI_ConceptParams* conceptParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_text_get_ranked_concepts_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, conceptParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetRankedKeywords(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_ranked_keywords, url);
    }
    const std::string& URLGetRankedKeywords(const std::string& url, AlchemyAPI_KeywordParams* keywordParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_ranked_keywords_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, keywordParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetRankedKeywords(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_ranked_keywords, html, url);
    }
    const std::string& HTMLGetRankedKeywords(
        const std::string& html, const std::string& url, AlchemyAPI_KeywordParams* keywordParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_ranked_keywords_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, keywordParams->getParameters()));

        return buffer;
    }
	
    const std::string& TextGetRankedKeywords(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_ranked_keywords, text);
    }
    const std::string& TextGetRankedKeywords(const std::string& text, AlchemyAPI_KeywordParams* keywordParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_text_get_ranked_keywords_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, keywordParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetLanguage(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_language, url);
    }
    const std::string& URLGetLanguage(const std::string& url, AlchemyAPI_LanguageParams* languageParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_language_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, languageParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetLanguage(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_language, html, url);
    }
    const std::string& HTMLGetLanguage(
        const std::string& html, const std::string& url, AlchemyAPI_LanguageParams* languageParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_language_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, languageParams->getParameters()));

        return buffer;
    }
	
    const std::string& TextGetLanguage(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_language, text);
    }
    const std::string& TextGetLanguage(const std::string& text, AlchemyAPI_LanguageParams* languageParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_text_get_language_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, languageParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetCategory(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_category, url);
    }
    const std::string& URLGetCategory(const std::string& url, AlchemyAPI_CategoryParams* categoryParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_category_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, categoryParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetCategory(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_category, html, url);
    }
    const std::string& HTMLGetCategory(
        const std::string& html, const std::string& url, AlchemyAPI_CategoryParams* categoryParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_category_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, categoryParams->getParameters()));

        return buffer;
    }
	
    const std::string& TextGetCategory(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_category, text);
    }
    const std::string& TextGetCategory(const std::string& text, AlchemyAPI_CategoryParams* categoryParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_text_get_category_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, categoryParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetText(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_text, url);
    }
    const std::string& URLGetText(const std::string& url, AlchemyAPI_TextParams* textParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_text_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, textParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetText(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_text, html, url);
    }
    const std::string& HTMLGetText(
        const std::string& html, const std::string& url, AlchemyAPI_TextParams* textParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_text_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, textParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetRawText(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_raw_text, url);
    }
    const std::string& URLGetRawText(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_raw_text_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetRawText(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_raw_text, html, url);
    }
    const std::string& HTMLGetRawText(
        const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_raw_text_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetTitle(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_title, url);
    }
    const std::string& URLGetTitle(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_title_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetTitle(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_title, html, url);
    }
    const std::string& HTMLGetTitle(
        const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_title_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetFeedLinks(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_feed_links, url);
    }
    const std::string& URLGetFeedLinks(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_feed_links_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetFeedLinks(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_feed_links, html, url);
    }
    const std::string& HTMLGetFeedLinks(
        const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_feed_links_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetMicroformats(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_microformats, url);
    }
    const std::string& URLGetMicroformats(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_url_get_microformats_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetMicroformats(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_microformats, html, url);
    }
    const std::string& HTMLGetMicroformats(
        const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_html_get_microformats_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetConstraintQuery(
        const std::string& url, const std::string& query)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_constraint_query(
            &apiObj, url.c_str(), query.c_str(), AlchemyRetCallback, this));

        return buffer;
    }
    const std::string& URLGetConstraintQuery(
        const std::string& url, const std::string& query, AlchemyAPI_ConstraintQueryParams* cqParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_constraint_query_params(
            &apiObj, url.c_str(), query.c_str(), AlchemyRetCallback, this, cqParams->getParameters()));

        return buffer;
    }
	
    const std::string& HTMLGetConstraintQuery(
        const std::string& html, const std::string& url,
        const std::string& query)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_constraint_query(
            &apiObj, html.c_str(), url.c_str(), query.c_str(),
            AlchemyRetCallback, this));

        return buffer;
    }
    const std::string& HTMLGetConstraintQuery(
        const std::string& html, const std::string& url,
        const std::string& query, AlchemyAPI_ConstraintQueryParams* cqParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_constraint_query_params(
            &apiObj, html.c_str(), url.c_str(), query.c_str(),
            AlchemyRetCallback, this, cqParams->getParameters()));

        return buffer;
    }
	
	const std::string& URLGetTextSentiment(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_textsentiment, url);
    }
    const std::string& URLGetTextSentiment(const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	    buffer.clear();
        CheckRetCode(alchemy_url_get_textsentiment_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
	const std::string& HTMLGetTextSentiment(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_textsentiment, html, url);
    }
    const std::string& HTMLGetTextSentiment(
        const std::string& html, const std::string& url, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	    buffer.clear();
        CheckRetCode(alchemy_html_get_textsentiment_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }
	
	const std::string& TextGetTextSentiment(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_textsentiment, text);
    }
    const std::string& TextGetTextSentiment(const std::string& text, AlchemyAPI_BaseParams* baseParams)
        throw (std::runtime_error, std::invalid_argument)
    {
	buffer.clear();
        CheckRetCode(alchemy_text_get_textsentiment_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, baseParams->getParameters()));

        return buffer;
    }

    const std::string& TextGetTargetedSentiment(const std::string& text, const std::string& target)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_targeted_sentiment, text, target);
    }

    const std::string& TextGetTargetedSentiment(const std::string& text, const std::string& target, AlchemyAPI_TargetedSentimentParams* targetedSentimentParams)
	 throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
	CheckRetCode(alchemy_text_get_targeted_sentiment_params(
	    &apiObj, text.c_str(), target.c_str(), 
	    AlchemyRetCallback, this, targetedSentimentParams->getParameters()));

	return buffer;
    }

    const std::string& URLGetTargetedSentiment(const std::string& url, const std::string& target)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_targeted_sentiment, url, target);
    }
    
    const std::string& URLGetTargetedSentiment(const std::string& url, const std::string& target, AlchemyAPI_TargetedSentimentParams* targetedSentimentParams)
	 throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
	CheckRetCode(alchemy_url_get_targeted_sentiment_params(
	    &apiObj, url.c_str(), target.c_str(), 
	    AlchemyRetCallback, this, targetedSentimentParams->getParameters()));

	return buffer;
    
    }

    const std::string& HTMLGetTargetedSentiment(const std::string& html, const std::string& url, const std::string& target)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_targeted_sentiment, html, url, target);
    }

    const std::string& HTMLGetTargetedSentiment(const std::string& html, const std::string& url, const std::string& target, AlchemyAPI_TargetedSentimentParams* targetedSentimentParams)
	 throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
	CheckRetCode(alchemy_html_get_targeted_sentiment_params(
	    &apiObj, html.c_str(), url.c_str(), target.c_str(), 
	    AlchemyRetCallback, this, targetedSentimentParams->getParameters()));

	return buffer;
    
    } 

    const std::string& URLGetRankedTaxonomy(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_ranked_taxonomy, url);
    }
    const std::string& URLGetRankedTaxonomy(const std::string& url, 
				            AlchemyAPI_TaxonomyParams* taxonomyParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_taxonomy_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, taxonomyParams->getParameters()));

        return buffer;
    }

    const std::string& HTMLGetRankedTaxonomy(
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_html_get_ranked_taxonomy, html, url);
    }
    const std::string& HTMLGetRankedTaxonomy(
        const std::string& html, const std::string& url, AlchemyAPI_TaxonomyParams* taxonomyParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_html_get_taxonomy_params(
            &apiObj, html.c_str(), url.c_str(),
            AlchemyRetCallback, this, taxonomyParams->getParameters()));

        return buffer;
    }

    const std::string& TextGetRankedTaxonomy(const std::string& text)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_text_get_ranked_taxonomy, text);
    }
    const std::string& TextGetRankedTaxonomy(const std::string& text, 
				             AlchemyAPI_TaxonomyParams* taxonomyParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_text_get_taxonomy_params(
            &apiObj, text.c_str(),
            AlchemyRetCallback, this, taxonomyParams->getParameters()));

        return buffer;
    }


    const std::string& URLGetImage(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_image, url);
    }
    const std::string& URLGetImage(const std::string& url, AlchemyAPI_ImageParams* imageParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_image_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, imageParams->getParameters()));

        return buffer;
    }

    const std::string& URLGetCombinedData(const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        return PerformRequest(alchemy_url_get_combined_data, url);
    }
    const std::string& URLGetCombinedData(const std::string& url, 
				          AlchemyAPI_CombinedDataParams* combinedDataParams)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemy_url_get_combined_data_params(
            &apiObj, url.c_str(),
            AlchemyRetCallback, this, combinedDataParams->getParameters()));

        return buffer;
    }

private:
    typedef Alchemy_code (*ALCHEMY_TEXT_OR_URL_ROUTINE)
        (const alchemyAPI_obj* api_obj, const char* data1,
         alchemy_ret_callback callback, void* data);
	typedef Alchemy_code (*ALCHEMY_TEXT_OR_URL_PARAMETERS_ROUTINE)
        (const alchemyAPI_obj* api_obj, const char* data1,
         alchemy_ret_callback callback, void* data, AlchemyAPI_KeywordParams* keywordParams);
    typedef Alchemy_code (*ALCHEMY_HTML_ROUTINE)
        (const alchemyAPI_obj* api_obj, const char* html, const char* url,
         alchemy_ret_callback callback, void* data);
    typedef Alchemy_code (*ALCHEMY_HTML_PARAMETERS_ROUTINE)
        (const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* required_arg,
         alchemy_ret_callback callback, void* data);


    alchemyAPI_obj apiObj;
    std::string buffer;

    const std::string& PerformRequest(
        ALCHEMY_TEXT_OR_URL_ROUTINE alchemyRoutine, const std::string& data)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemyRoutine(
            &apiObj, data.c_str(), AlchemyRetCallback, this));

        return buffer;
    }
	

    const std::string& PerformRequest(
        ALCHEMY_HTML_ROUTINE alchemyRoutine,
        const std::string& html, const std::string& url)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemyRoutine(
            &apiObj, html.c_str(), url.c_str(), AlchemyRetCallback, this));

        return buffer;
    }

    const std::string& PerformRequest(
        ALCHEMY_HTML_PARAMETERS_ROUTINE alchemyRoutine,
        const std::string& html, const std::string& url, const std::string& required_arg)
        throw (std::runtime_error, std::invalid_argument)
    {
        buffer.clear();
        CheckRetCode(alchemyRoutine(
            &apiObj, html.c_str(), url.c_str(), required_arg.c_str(), AlchemyRetCallback, this));

        return buffer;
    }
    

    void CheckRetCode(Alchemy_code code)
        throw (std::runtime_error, std::invalid_argument)
    {
        try
        {
            switch (code)
            {
                case ALCHEMY_OK:
                    break;
                case ALCHEMY_ILLEGAL_KEY_SPECIFIED:
                    throw std::invalid_argument("Illegal API key specified");
                case ALCHEMY_ILLEGAL_URL_SPECIFIED:
                    throw std::invalid_argument("Illegal URL specified");
                case ALCHEMY_ILLEGAL_HTML_SPECIFIED:
                    throw std::invalid_argument(
                        "Illegal HTML document specified");
                case ALCHEMY_ILLEGAL_TEXT_SPECIFIED:
                    throw std::invalid_argument("Illegal text specified");
                case ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED:
                     throw std::invalid_argument(
                         "Insufficient parameters specified");
                case ALCHEMY_UNABLE_TO_READ_KEY:
                    throw std::runtime_error("Unable to load API key");
                case ALCHEMY_CURL_INIT_FAILED:
                    throw std::runtime_error("CURL initialization failed");
                case ALCHEMY_REQUEST_INIT_FAILED:
                    throw std::runtime_error(
                        "Alchemy request initialization failed");
                case ALCHEMY_REQUEST_PERFORM_FAILED:
                    throw std::runtime_error(
                        "Performing of Alchemy request failed");
                case ALCHEMY_ILLEGAL_ALCHEMY_RESPONSE:
                    throw std::runtime_error("Illegal Alchemy response");
                case ALCHEMY_INTERNAL_ERROR:
                default:
                    throw std::runtime_error("Internal API runtime error");
            }
        } catch (...)
        {
            buffer.clear();
            throw;
        }
    }

    static void AlchemyRetCallback(const char* buf, size_t len, void* data)
    {
        if (buf && len && data)
        {
            AlchemyFacade* facade = static_cast<AlchemyFacade*>(data);
            if (facade)
                facade->buffer.append(buf, len);
        }
    }
};

} // namespace alchemy

#endif // __ALCHEMY_API_HPP__
