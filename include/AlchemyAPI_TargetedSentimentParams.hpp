/*************************************************************************
 * Summary: AlchemyAPI TargetedSentimentParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/
 
#ifndef __ALCHEMYAPI_TARGETEDSENTIMENTPARAMS_HPP__
#define __ALCHEMYAPI_TARGETEDSENTIMENTPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{
	class AlchemyAPI_TargetedSentimentParams
	{
		public:
			AlchemyAPI_TargetedSentimentParams() {
				params = alchemy_targeted_sentiment_param_init();
			}
			
			~AlchemyAPI_TargetedSentimentParams() {
				alchemy_targeted_sentiment_param_free( &params );
			}
			
			Alchemy_targeted_sentiment_params* getParameters() 
			{
				return &params;
			}
			
			void setShowSourceText(bool showSourceText) {
				if( showSourceText )
					alchemy_targeted_sentiment_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_targeted_sentiment_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_FALSE);
			}
			
			bool getShowSourceText() const {
				return (bool)(params.show_source_text-1);
			}

			std::string getSentimentPhrase() const {
                return params.sentiment_phrase;
            }

            void setSentimentPhrase(const std::string& sentimentPhrase) {
                alchemy_targeted_sentiment_param_set_string( &params, ALCHEMY_PARAMETERS_SENTIMENT_PHRASE, sentimentPhrase.c_str());
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
			Alchemy_targeted_sentiment_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_TARGETEDSENTIMENTPARAMS_HPP__
