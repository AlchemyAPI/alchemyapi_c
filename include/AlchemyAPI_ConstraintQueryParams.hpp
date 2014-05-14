/*************************************************************************
 * Summary: AlchemyAPI ConstraintQueryParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_CONSTRAINTQUERYPARAMS_HPP__
#define __ALCHEMYAPI_CONSTRAINTQUERYPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_ConstraintQueryParams
	{
		public:
			AlchemyAPI_ConstraintQueryParams() {
				params = alchemy_constraint_query_param_init();
			}
			
			~AlchemyAPI_ConstraintQueryParams() {
				alchemy_constraint_query_param_free( &params );
			}
			
			Alchemy_constraint_query_params* getParameters() 
			{
				return &params;
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_constraint_query_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
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
			Alchemy_constraint_query_params params;
	
	};
}

#endif // __ALCHEMYAPI_CONSTRAINTQUERYPARAMS_HPP__
