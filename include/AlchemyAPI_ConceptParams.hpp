/*************************************************************************
 * Summary: AlchemyAPI ConceptParams C++ Wrapper
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMYAPI_CONCEPTPARAMS_HPP__
#define __ALCHEMYAPI_CONCEPTPARAMS_HPP__

#include <string>
#include <stdexcept>

#include "alchemy_api.h"

namespace alchemy
{

	class AlchemyAPI_ConceptParams
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
			

			AlchemyAPI_ConceptParams()
			{
				params = alchemy_concept_param_init();
			}
			
			~AlchemyAPI_ConceptParams()
			{
				alchemy_concept_param_free(&params);
			}
			
			Alchemy_concept_params* getParameters() 
			{
				return &params;
			}
			
			int getMaxRetrieve() const
			{
				return params.max_retrieve;
			}
			
			void setMaxRetrieve(int maxRetrieve) 
			{
				alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, maxRetrieve);
			}
			
			bool getShowSourceText() const {
				return (bool)(params.show_source_text-1);
			}
			
			void setShowSourceText(bool showSourceText) {
				if( showSourceText )
					alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_FALSE);
			}
			
			SourceTextMode getSourceText() const {
				return (SourceTextMode)params.source_text;
			}
			
			void setSourceText(SourceTextMode sourceText) {
				alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_SOURCE_TEXT, sourceText);
			}
			
			bool getLinkedData() const {
				return (bool)(params.linked_data-1);
			}
			
			void setlinkedData(bool linkedData) {
				if( linkedData )
					alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_TRUE);
				else 
					alchemy_concept_param_set_int(&params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_FALSE);
			}
			
			std::string getCQuery() const {
				return params.cquery;
			}
			
			void setCQuery(const std::string& cQuery) {
				alchemy_concept_param_set_string( &params, ALCHEMY_PARAMETERS_CQUERY, cQuery.c_str() );
			}
			
			std::string getXPath() const {
				return params.xpath;
			}
			
			void setXPath(const std::string& xPath) {
				alchemy_concept_param_set_string( &params, ALCHEMY_PARAMETERS_XPATH, xPath.c_str() );
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
			Alchemy_concept_params params;
	
	};
	
}

#endif // __ALCHEMYAPI_CONCEPTPARAMS_HPP__
