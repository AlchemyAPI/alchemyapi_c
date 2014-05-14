/*************************************************************************
 * Summary: AlchemyAPI KeywordParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMYAPI_BASEPARAMS_HPP__
#define __ALCHEMYAPI_BASEPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{

	class AlchemyAPI_BaseParams
	{
		public:
			AlchemyAPI_BaseParams()
			{
				params = alchemy_base_param_init();
			}
			
			~AlchemyAPI_BaseParams()
			{
				alchemy_base_param_free(&params);
			}
			
			Alchemy_base_params* getParameters() 
			{
				return &params;
			}
			
			std::string getCustomParameters() const
			{
				return params.custom_params;
			}
			
			void setCustomParameters(int params_num, ...) 
			{
				va_list temp_list;
				va_start(temp_list, params_num);
					
				alchemy_set_custom_params( &params, params_num, temp_list);
			}
			
		private:
			Alchemy_base_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_BASEPARAMS_HPP__
