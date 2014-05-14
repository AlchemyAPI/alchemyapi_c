/*************************************************************************
 * Summary: AlchemyAPI Example - Named Entity Extraction
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#include <stdio.h>

#include "alchemy_api.h"
#include "utils.h"

int main(int argc, char** argv)
{
	char* html = NULL;
	alchemyAPI_obj api_obj = { 0U };

	if (ALCHEMY_OK != alchemy_global_init())
        	alchemy_error_exit("Unable to initialize Alchemy API", NULL, html);
	if (ALCHEMY_OK != alchemy_load_key_util(&api_obj, "data/api_key.txt"))
        	alchemy_error_exit("Unable to load Alchemy API key", &api_obj, html);
	if (!alchemy_load_data(&html, "data/example.html"))
        	alchemy_error_exit("Unable to load 'data/example.html' file", &api_obj, html);
						 
	Alchemy_entity_params entity_params = alchemy_entity_param_init();
	//use 'set_int' functions to set integer parameters (maxRetrieve), as well as any parameters that accept enums as arguments (like disambiguate, sourceText)
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, 1);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_DISAMBIGUATE, ALCHEMY_BOOL_TRUE);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_LINKED_DATA, ALCHEMY_BOOL_TRUE);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_COREFERENCE, ALCHEMY_BOOL_TRUE);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_QUOTATIONS, ALCHEMY_BOOL_TRUE);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_SOURCE_TEXT, ALCHEMY_XPATH);
	alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
	//use 'set_string' functions to set string parameters
	alchemy_entity_param_set_string( &entity_params, ALCHEMY_PARAMETERS_XPATH, "//a");
	
	fprintf(stdout, "\nExtracting a ranked list of named entities from an "
                    "URL with parameters\n");
	if (ALCHEMY_OK != alchemy_url_get_ranked_named_entities_params(
                    &api_obj, "http://www.techcrunch.com/",
                    alchemy_stdout_callback, NULL, &entity_params))
	        alchemy_error_exit("alchemy_url_get_ranked_named_entities invocation "
        	                 "failed", &api_obj, html);

	alchemy_entity_param_free( &entity_params );
	
	
	Alchemy_keyword_params keyword_params = alchemy_keyword_param_init();
	alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_MAX_RETRIEVE, 1);
	alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE, ALCHEMY_STRICT);
	alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_SOURCE_TEXT, ALCHEMY_RAW);
	alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);
	alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE );
	alchemy_keyword_param_set_custom_params( &keyword_params, 1u, "testparam", "testparam");

	fprintf(stdout, "\nExtracting a ranked list of keywords from text "
                    "with parameters\n");
	if (ALCHEMY_OK != alchemy_text_get_ranked_keywords_params(&api_obj,
                    "Hello there, my name is Bob Jones.  I live in the United "
                    "States of America.  Where do you live, Fred?",
                    alchemy_stdout_callback, NULL, &keyword_params))
	        alchemy_error_exit("alchemy_text_get_ranked_keywords invocation failed",
        	                 &api_obj, html);
						 
	alchemy_keyword_param_free( &keyword_params );


	fprintf(stdout, "\nExtracting sentiment from text\n");
	if (ALCHEMY_OK != alchemy_text_get_textsentiment(&api_obj,
                    "Hello there, my name is Bob Jones.  I live in the United "
                    "States of America.  Where do you live, Fred?",
                    alchemy_stdout_callback, NULL))
	        alchemy_error_exit("alchemy_text_get_textsentiment invocation failed",
        	                 &api_obj, html);
	
	
	Alchemy_base_params base_params = alchemy_base_param_init();
	alchemy_base_param_set_custom_params( &base_params, 1u, "showSourceText", "1");
	fprintf(stdout, "\nExtracting sentiment from text\n");

	if (ALCHEMY_OK != alchemy_html_get_textsentiment(&api_obj,
				"<html><body>This is text</body></html>", "http://www.sample.com",
				alchemy_stdout_callback, NULL))
		alchemy_error_exit("alchemy_text_get_textsentiment invocation failed",
						 &api_obj, html);
						 
	alchemy_base_param_free( &base_params );
					 
	alchemy_exit(&api_obj, html);
}
