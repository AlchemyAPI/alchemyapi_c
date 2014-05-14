/*************************************************************************
 * Summary: AlchemyAPI TextParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_TEXTPARAMS_HPP__
#define __ALCHEMYAPI_TEXTPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_TextParams
	{
		public:
			AlchemyAPI_TextParams() {
				params = alchemy_text_param_init();
			}
			
			~AlchemyAPI_TextParams() {
				alchemy_text_param_free( &params );
			}
			
			bool getExtractLinks() const {
				return (bool)(params.extract_links-1);
			}

			Alchemy_text_params* getParameters() 
			{
				return &params;
			}
			
			void setExtractLinks(bool extractLinks) {
				if( extractLinks )
					alchemy_text_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_text_param_set_int(&params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_FALSE);
			}
			
			bool getUseMetadata() const {
				return (bool)(params.use_metadata-1);
			}
			
			void setUseMetadata(bool useMetadata) {
				if( useMetadata )
					alchemy_text_param_set_int(&params, ALCHEMY_PARAMETERS_USE_METADATA, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_text_param_set_int(&params, ALCHEMY_PARAMETERS_USE_METADATA, ALCHEMY_BOOL_FALSE);
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
			Alchemy_text_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_LANGUAGEPARAMS_HPP__
