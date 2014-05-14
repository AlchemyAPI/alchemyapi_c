/*************************************************************************
 * Summary: AlchemyAPI Example - Author Extraction
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

    fprintf(stdout, "\nExtracting author from an URL.\n");
    if (ALCHEMY_OK != alchemy_url_get_author(&api_obj, "http://www.politico.com/blogs/media/2012/02/detroit-news-ed-upset-over-romney-edit-115247.html",
                                                  alchemy_stdout_callback, NULL))
        alchemy_error_exit("alchemy_url_get_author invocation failed",
                         &api_obj, html);

    alchemy_exit(&api_obj, html);
}
