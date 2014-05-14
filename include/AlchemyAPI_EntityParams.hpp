/*************************************************************************
 * Summary: AlchemyAPI EntityParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_ENTITYPARAMS_HPP__
#define __ALCHEMYAPI_ENTITYPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_EntityParams
	{
		public:
			enum SourceTextMode {
				STM_NONE,
				CLEANED_OR_RAW,
				CLEANED,
				RAW,
				CQUERY,
				XPATH
			};
		
			AlchemyAPI_EntityParams() {
				params = alchemy_entity_param_init();
			}
			
			~AlchemyAPI_EntityParams() {
				alchemy_entity_param_free(&params);
			}
			
			Alchemy_entity_params* getParameters() 
			{
				return &params;
			}
			
			int getMaxRetrieve() const
			{
				return params.max_retrieve;
			}
			
			void setMaxRetrieve(int maxRetrieve) 
			{
				alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, maxRetrieve);
			}

			bool getDisambiguate() const {
				return (bool)(params.disambiguate-1);
			}
			
			void setDisambiguate(bool disambiguate) {
				if( disambiguate )
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_FALSE);
			}
			
			bool getQuotations() const {
				return (bool)(params.quotations-1);
			}
			
			void setQuotations(bool quotations) {
				if( quotations )
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_QUOTATIONS, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_QUOTATIONS, ALCHEMY_BOOL_FALSE);
			}
			
			bool getCoreference() const {
				return (bool)(params.coreference-1);
			}
			
			void setCoreference(bool coreference) {
				if( coreference )
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_COREFERENCE, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_COREFERENCE, ALCHEMY_BOOL_FALSE);
			}
			
			bool getLinkedData() const {
				return (bool)(params.linked_data-1);
			}
			
			void setlinkedData(bool linkedData) {
				if( linkedData )
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_FALSE);
			}
			
			bool getShowSourceText() const {
				return (bool)(params.show_source_text-1);
			}
			
			void setShowSourceText(bool showSourceText) {
				if( showSourceText )
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_FALSE);
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceText) {
				alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceText);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_entity_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_entity_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
			}
			
			std::string getBaseUrl() const {
				return params.base_url;
			}
			
			void setBaseUrl(const std::string& baseUrl) {
				alchemy_entity_param_set_string( &params, ALCHEMY_PARAMETERS_BASE_URL, baseUrl.c_str());
			}

			bool getSentiment() const {
                                return (bool)(params.sentiment-1);
                        }

                        void setSentiment(bool sentiment) {
                                if( sentiment )
                                        alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE);
                                else
                                        alchemy_entity_param_set_int(&params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_FALSE);
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
			Alchemy_entity_params params;
	
	};
}

#endif // __ALCHEMYAPI_ENTITYPARAMS_HPP__
