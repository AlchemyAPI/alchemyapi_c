/*************************************************************************
 * Summary: AlchemyAPI ImageParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMYAPI_IMAGEPARAMS_HPP__
#define __ALCHEMYAPI_IMAGEPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{

	class AlchemyAPI_ImageParams
	{
		public:
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
			
			AlchemyAPI_ImageParams()
			{
				params = alchemy_image_param_init();
			}
			
			~AlchemyAPI_ImageParams()
			{
				alchemy_image_param_free(&params);
			}
			
			Alchemy_image_params* getParameters()
			{
				return &params;
			}
			
			ImageExtractMode getImageExtractMode() const {
				return (ImageExtractMode)params.image_extract_mode;
			}
			
			void setImageExtractMode(ImageExtractMode iem) {
				alchemy_image_param_set_int(&params, ALCHEMY_PARAMETERS_IMAGE_EXTRACT_MODE, iem);
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
			Alchemy_image_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_IMAGEPARAMS_HPP__
