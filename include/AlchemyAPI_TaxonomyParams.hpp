/*************************************************************************
 * Summary: AlchemyAPI TaxonomyParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMYAPI_TAXONOMYPARAMS_HPP__
#define __ALCHEMYAPI_TAXONOMYPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{

	class AlchemyAPI_TaxonomyParams
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
			

			AlchemyAPI_TaxonomyParams()
			{
				params = alchemy_taxonomy_param_init();
			}
			
			~AlchemyAPI_TaxonomyParams()
			{
				alchemy_taxonomy_param_free(&params);
			}
			
			Alchemy_taxonomy_params* getParameters()
			{
				return &params;
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceText) {
				alchemy_taxonomy_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceText);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_taxonomy_param_set_string(&params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_taxonomy_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
			}

            std::string getBaseUrl() const {
                return params.base_url;
            }

            void setBaseUrl(const std::string& baseUrl) {
                alchemy_taxonomy_param_set_string( &params, ALCHEMY_PARAMETERS_BASE_URL, baseUrl.c_str());
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
			Alchemy_taxonomy_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_TAXONOMYPARAMS_HPP__
