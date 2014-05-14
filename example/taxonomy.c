/*************************************************************************
 * Summary: AlchemyAPI Example - Taxonomy Classification
 * Copyright: (C) 2014 AlchemyAPI, LLC - See COPYING file.
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

    fprintf(stdout, "\nRetrieving taxonomy classification of a URL.\n");
    if (ALCHEMY_OK != alchemy_url_get_ranked_taxonomy(&api_obj, "http://www.techcrunch.com/",
                                                      alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_url_get_ranked_taxonomy invocation failed",
                         &api_obj, html);

    fprintf(stdout, "\nRetrieving taxonomy classification of some text.\n");
    if (ALCHEMY_OK != alchemy_text_get_ranked_taxonomy(&api_obj, "Latest on the War in Iraq.",
                                                       alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_text_get_ranked_taxonomy invocation failed",
                         &api_obj, html);

    if (!alchemy_load_data(&html, "data/example.html"))
        alchemy_error_exit("Unable to load 'data/example.html' file", &api_obj, html);

    fprintf(stdout, "\nRetrieving taxonomy classification of a HTML document.\n");
    if (ALCHEMY_OK != alchemy_html_get_ranked_taxonomy(&api_obj, html, "http://www.test.com/",
                                                alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_html_get_ranked_taxonomy invocation failed",
                         &api_obj, html);

    alchemy_exit(&api_obj, html);
}
