/*************************************************************************
 * Summary: AlchemyAPI CombinedDataParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_COMBINEDDATAPARAMS_HPP__
#define __ALCHEMYAPI_COMBINEDDATAPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_CombinedDataParams
	{
		public:
            enum KeywordExtractMode {
                KEM_NONE,
                NORMAL,
                STRICT
            };

            enum ImageExtractMode {
                ALWAYS_INFER,
                TRUST_META
            };

			enum SourceTextMode {
				STM_NONE,
				CLEANED_OR_RAW,
				CLEANED,
				RAW,
				CQUERY,
				XPATH
			};
		
			AlchemyAPI_CombinedDataParams() {
				params = alchemy_combined_data_param_init();
			}
			
			~AlchemyAPI_CombinedDataParams() {
				alchemy_combined_data_param_free(&params);
			}
			
			Alchemy_combined_data_params* getParameters()
			{
				return &params;
			}
			
			int getMaxRetrieve() const
			{
				return params.max_retrieve;
			}
			
			void setMaxRetrieve(int maxRetrieve) 
			{
				alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, maxRetrieve);
			}

			bool getDisambiguate() const {
				return (bool)(params.disambiguate-1);
			}
			
			void setDisambiguate(bool disambiguate) {
				if( disambiguate )
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_FALSE);
			}
			
			bool getQuotations() const {
				return (bool)(params.quotations-1);
			}
			
			void setQuotations(bool quotations) {
				if( quotations )
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_QUOTATIONS, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_QUOTATIONS, ALCHEMY_BOOL_FALSE);
			}
			
			bool getCoreference() const {
				return (bool)(params.coreference-1);
			}
			
			void setCoreference(bool coreference) {
				if( coreference )
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_COREFERENCE, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_COREFERENCE, ALCHEMY_BOOL_FALSE);
			}
			
			bool getLinkedData() const {
				return (bool)(params.linked_data-1);
			}
			
			void setlinkedData(bool linkedData) {
				if( linkedData )
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_FALSE);
			}
			
			bool getShowSourceText() const {
				return (bool)(params.show_source_text-1);
			}
			
			void setShowSourceText(bool showSourceText) {
				if( showSourceText )
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_FALSE);
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceText) {
				alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceText);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_combined_data_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_combined_data_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
			}
			
			std::string getBaseUrl() const {
				return params.base_url;
			}
			
			void setBaseUrl(const std::string& baseUrl) {
				alchemy_combined_data_param_set_string( &params, ALCHEMY_PARAMETERS_BASE_URL, baseUrl.c_str());
			}

            KeywordExtractMode getKeywordExtractMode() const {
                return (KeywordExtractMode)params.keyword_extract_mode;
            }

            void setKeywordExtractMode(KeywordExtractMode kem) {
                alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE, kem);
            }

            bool getUseMetadata() const {
                return (bool)(params.use_metadata-1);
            }

            void setUseMetadata(bool use_metadata) {
                if( use_metadata )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_USE_METADATA, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_USE_METADATA, ALCHEMY_BOOL_FALSE);
            }

            bool getExtractLinks() const {
                return (bool)(params.extract_links-1);
            }

            void setExtractLinks(bool extract_links) {
                if( extract_links )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_EXTRACT_LINKS, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_EXTRACT_LINKS, ALCHEMY_BOOL_FALSE);
            }

            std::string getSentimentPhrase() const {
                return params.sentiment_phrase;
            }

            void setSentimentPhrase(const std::string& sentimentPhrase) {
                alchemy_combined_data_param_set_string( &params, ALCHEMY_PARAMETERS_SENTIMENT_PHRASE, sentimentPhrase.c_str());
            }

			bool getSentiment() const {
                return (bool)(params.sentiment-1);
            }

            void setSentiment(bool sentiment) {
                if( sentiment )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_FALSE);
            }

            bool getEntities() const {
                return (bool)(params.entities-1);
            }

            void setEntities(bool entities) {
                if( entities )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_ENTITIES, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_ENTITIES, ALCHEMY_BOOL_FALSE);
            }

            bool getRequiredEntities() const {
                return (bool)(params.required_entities-1);
            }

            void setRequiredEntities(bool required_entities) {
                if( required_entities )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_REQUIRED_ENTITIES, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_REQUIRED_ENTITIES, ALCHEMY_BOOL_FALSE);
            }

            bool getExcludeEntities() const {
                return (bool)(params.sentiment_exclude_entities-1);
            }

            void setExcludeEntities(bool sentiment_exclude_entities) {
                if( sentiment_exclude_entities )
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_EXCLUDE_SENTI_ENTITIES, ALCHEMY_BOOL_TRUE);
                else
                    alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_EXCLUDE_SENTI_ENTITIES, ALCHEMY_BOOL_FALSE);
            }

            ImageExtractMode getImageExtractMode() const {
                return (ImageExtractMode)params.keyword_extract_mode;
            }

            void setImageExtractMode(ImageExtractMode iem) {
                alchemy_combined_data_param_set_int(&params, ALCHEMY_PARAMETERS_IMAGE_EXTRACT_MODE, iem);
            }

            std::string getExtract() const {
                return params.extract;
            }

            void setExtract(const std::string& extract) {
                alchemy_combined_data_param_set_string( &params, ALCHEMY_PARAMETERS_EXTRACT, extract.c_str());
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
			Alchemy_combined_data_params params;
	
	};
}

#endif // __ALCHEMYAPI_COMBINEDDATAPARAMS_HPP__
