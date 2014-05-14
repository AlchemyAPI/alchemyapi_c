/*************************************************************************
 * Summary: AlchemyAPI Example - Sentiment Extraction
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

    fprintf(stdout, "\nExtracting sentiment from an URL.\n");
    if (ALCHEMY_OK != alchemy_url_get_textsentiment(&api_obj, "http://www.techcrunch.com/",
                                                  alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_url_get_textsentiment invocation failed",
                         &api_obj, html);

    fprintf(stdout, "\nExtracting sentiment from a HTML document.\n");
    if (ALCHEMY_OK != alchemy_html_get_textsentiment(&api_obj, html, "http://www.test.com/",
                                                   alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_html_get_textsentiment invocation failed",
                         &api_obj, html);

    fprintf(stdout, "\nExtracting sentiment from a text string.\n");
    if (ALCHEMY_OK != alchemy_text_get_textsentiment(&api_obj,
                    "Hello there, my name is Bob Jones.  I live in the United "
                    "States of America.  Where do you live, Fred?",
                    alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_text_get_textsentiment invocation failed",
                         &api_obj, html);

    Alchemy_entity_params entity_params = alchemy_entity_param_init();
    alchemy_entity_param_set_int( &entity_params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE);

    fprintf(stdout, "\nExtracting named entities from an "
                    "URL with entity-targeted sentiment enabled\n");
    if (ALCHEMY_OK != alchemy_url_get_ranked_named_entities_params(
                    &api_obj, "http://www.techcrunch.com/",
                    alchemy_stdout_callback, NULL, &entity_params))
    alchemy_error_exit("alchemy_url_get_ranked_named_entities invocation "
                       "failed", &api_obj, html);

    alchemy_entity_param_free( &entity_params );
	
	
    Alchemy_keyword_params keyword_params = alchemy_keyword_param_init();
    alchemy_keyword_param_set_int( &keyword_params, ALCHEMY_PARAMETERS_SENTIMENT, ALCHEMY_BOOL_TRUE);

    fprintf(stdout, "\nExtracting keywords from an "
                    "URL with keyword-targeted sentiment enabled\n");
    if (ALCHEMY_OK != alchemy_url_get_ranked_keywords_params(
                    &api_obj, "http://www.techcrunch.com/",
                    alchemy_stdout_callback, NULL, &keyword_params))
    alchemy_error_exit("alchemy_url_get_ranked_keywords invocation "
                       "failed", &api_obj, html);

    alchemy_keyword_param_free( &keyword_params );


    Alchemy_targeted_sentiment_params targeted_sentiment_params = alchemy_targeted_sentiment_param_init();
    alchemy_targeted_sentiment_param_set_int( &targeted_sentiment_params, ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT, ALCHEMY_BOOL_TRUE);

    fprintf(stdout, "\nExtracting sentiment from Text\n");
    if(ALCHEMY_OK != alchemy_text_get_targeted_sentiment( &api_obj, "This car is terrible.", "car", alchemy_stdout_callback, NULL))
	    alchemy_error_exit("alchemy_text_get_targeted_sentiment invocation failed", &api_obj, html);
    
    fprintf(stdout, "\nExtracting sentiment from URL\n");
    if(ALCHEMY_OK != alchemy_url_get_targeted_sentiment_params( &api_obj,
                          "http://techcrunch.com/2012/03/01/keen-on-anand-rajaraman-how-walmart-wants-to-leapfrog-over-amazon-tctv/",
			  "Walmart", alchemy_stdout_callback, NULL, &targeted_sentiment_params))
	    alchemy_error_exit("alchemy_url_get_targeted_sentiment invocation failed", &api_obj, html);

    fprintf(stdout, "\nExtracting sentiment from HTML\n");
    if(ALCHEMY_OK != alchemy_html_get_targeted_sentiment_params( &api_obj, html, "http://www.test.com", "twitter", alchemy_stdout_callback, NULL, &targeted_sentiment_params))
	    alchemy_error_exit("alchemy_html_get_targeted_sentiment invocation failed", &api_obj, html);

    alchemy_targeted_sentiment_param_free( &targeted_sentiment_params );
   
    alchemy_exit(&api_obj, html);
}
