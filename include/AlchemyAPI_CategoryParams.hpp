/*************************************************************************
 * Summary: AlchemyAPI CategoryParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_CATEGORYPARAMS_HPP__
#define __ALCHEMYAPI_CATEGORYPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_CategoryParams
	{
		public:
			enum SourceTextMode {
				STM_NONE,
				CLEANED_OR_RAW,
				CQUERY,
				XPATH
			};
		
			AlchemyAPI_CategoryParams() {
				params = alchemy_category_param_init();
			}
			
			~AlchemyAPI_CategoryParams() {
				alchemy_category_param_free(&params);
			}
			
			Alchemy_category_params* getParameters() 
			{
				return &params;
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceTextMode) {
				alchemy_category_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceTextMode);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_category_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_category_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
			}
			
			std::string getBaseUrl() const {
				return params.base_url;
			}
			
			void setBaseUrl(const std::string& baseUrl) {
				alchemy_category_param_set_string( &params, ALCHEMY_PARAMETERS_BASE_URL, baseUrl.c_str());
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
			Alchemy_category_params params;
	
	};

}

#endif // __ALCHEMYAPI_CATEGORYPARAMS_HPP__
