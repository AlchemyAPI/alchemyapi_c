/*************************************************************************
 * Summary: AlchemyAPI Example - Structured Content Scraping
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
    if (!alchemy_load_data(&html, "data/microformats.html"))
        alchemy_error_exit("Unable to load 'data/microformats.html' file",
                         &api_obj, html);

    fprintf(stdout, "\nExtracting first link from an URL.\n");
    if (ALCHEMY_OK != alchemy_url_get_constraint_query(&api_obj,
                    "http://microformats.org/wiki/hcard", "1st link",
                    alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_url_get_constraint_query invocation failed",
                         &api_obj, html);

    fprintf(stdout, "\nExtracting first link from a HTML document.\n");
    if (ALCHEMY_OK != alchemy_html_get_constraint_query(&api_obj, html,
                    "http://www.test.com/", "1st link",
                    alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_html_get_constraint_query invocation failed",
                         &api_obj, html);

    alchemy_exit(&api_obj, html);
}
