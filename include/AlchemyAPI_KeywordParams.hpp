/*************************************************************************
 * Summary: AlchemyAPI KeywordParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMYAPI_KEYWORDPARAMS_HPP__
#define __ALCHEMYAPI_KEYWORDPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{

	class AlchemyAPI_KeywordParams
	{
		public:
			enum KeywordExtractMode {
				KEM_NONE,
				NORMAL,
				STRICT
			};
			
			enum SourceTextMode {
				STM_NONE,
				CLEANED_OR_RAW,
				CLEANED,
				RAW,
				CQUERY,
				XPATH
			};
			

			AlchemyAPI_KeywordParams()
			{
				params = alchemy_keyword_param_init();
			}
			
			~AlchemyAPI_KeywordParams()
			{
				alchemy_keyword_param_free(&params);
			}
			
			Alchemy_keyword_params* getParameters() 
			{
				return &params;
			}
			
			int getMaxRetrieve() const
			{
				return params.max_retrieve;
			}
			
			void setMaxRetrieve(int maxRetrieve) 
			{
				alchemy_keyword_param_set_int(&params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, maxRetrieve);
			}
			
			KeywordExtractMode getKeywordExtractMode() const {
				return (KeywordExtractMode)params.keyword_extract_mode;
			}
			
			void setKeywordExtractMode(KeywordExtractMode kem) {
				alchemy_keyword_param_set_int(&params, ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE, kem);
			}
			
			bool getShowSourceText() const {
				return (bool)(params.show_source_text-1);
			}
			
			void setShowSourceText(bool showSourceText) {
				if( showSourceText )
					alchemy_keyword_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_keyword_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_FALSE);
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceText) {
				alchemy_keyword_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceText);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_keyword_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_keyword_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
			}
			
			std::string getBaseUrl() const {
				return params.base_url;
			}
			
			void setBaseUrl(const std::string& baseUrl) {
				alchemy_keyword_param_set_string( &params, ALCHEMY_PARAMETERS_BASE_URL, baseUrl.c_str());
			}
			
			std::string getCustomParameters() const
			{
				return params.base_params.custom_params;
			}
			
			void setCustomParameters(int params_num, ...) 
			{
				va_list temp_list;
				va_start(temp_list, params_num);
					
				alchemy_set_custom_params( &params.base_params, params_num, temp_list);
			}
			
			
		private:
			Alchemy_keyword_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_KEYWORDPARAMS_HPP__
