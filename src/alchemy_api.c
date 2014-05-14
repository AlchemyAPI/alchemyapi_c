/*************************************************************************
 * Summary: AlchemyAPI
 * Copyright: (C) 2014, AlchemyAPI, LLC - See COPYING file.
 *************************************************************************/

#include <string.h>
#include <libxml/parser.h>
#include <stdio.h>


#include "alchemy_api_int.h"


#define ALCHEMY_CHECK_BASIC_PARAMS(api_obj, callback) \
    if (NULL == callback) \
        return ALCHEMY_CALLBACK_ISNT_SPECIFIED; \
    if (!alchemy_check_key(api_obj)) \
        return ALCHEMY_ILLEGAL_KEY_SPECIFIED;

#define ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url) \
    ALCHEMY_CHECK_BASIC_PARAMS(api_obj, callback) \
    if (!alchemy_check_url(url)) \
        return ALCHEMY_ILLEGAL_URL_SPECIFIED;

#define ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text) \
    ALCHEMY_CHECK_BASIC_PARAMS(api_obj, callback) \
    if (!alchemy_check_text(text)) \
        return ALCHEMY_ILLEGAL_TEXT_SPECIFIED;


static void alchemy_trim_key(alchemyAPI_obj* api_obj)
{
    size_t new_len;
    const char* end = api_obj->key + api_obj->key_len - 1u;

    while (end > api_obj->key && ('\n' == *end || '\r' == *end))
        --end;

    new_len = end - api_obj->key + 1u;
    if (new_len != api_obj->key_len)
    {
        api_obj->key_len = new_len;
        api_obj->key = realloc((void*)api_obj->key, api_obj->key_len);
    }
}

Alchemy_code alchemy_global_init(void)
{
    if (CURLE_OK != curl_global_init(CURL_GLOBAL_ALL))
        return ALCHEMY_CURL_INIT_FAILED;

    xmlInitParser();

    return ALCHEMY_OK;
}

void alchemy_global_cleanup(void)
{
    curl_global_cleanup();
    xmlCleanupParser();
}

void alchemy_free_api_obj_util(alchemyAPI_obj* api_obj)
{
    alchemy_free_key_util(api_obj);
    alchemy_free_host_util(api_obj);
}

void alchemy_free_key_util(alchemyAPI_obj* api_obj)
{
    if (NULL != api_obj)
    {
        if (NULL != api_obj->key)
            free((void*)api_obj->key);
    }
    api_obj->key = NULL;
    api_obj->key_len = 0u;
}

void alchemy_free_host_util(alchemyAPI_obj* api_obj)
{
    if (NULL != api_obj)
    {
        if (NULL != api_obj->host)
            free((void*)api_obj->host);
    }
    api_obj->host = NULL;
    api_obj->host_len = 0u; 
}   
    

Alchemy_code alchemy_set_key_util(alchemyAPI_obj* api_obj, const char* keyText)
{
    long size = 0L;
    Alchemy_code ret = ALCHEMY_LAST;

    if (NULL == api_obj)
        return ALCHEMY_ILLEGAL_KEY_SPECIFIED;
    if (alchemy_is_empty(keyText))
        return ALCHEMY_ILLEGAL_KEY_SPECIFIED;

    size = strlen(keyText);
    if (size < 1)
        return ALCHEMY_ILLEGAL_KEY_SPECIFIED;

    api_obj->key = (char*)malloc(sizeof(char) * size);
    if (NULL != api_obj->key)
    {
        memcpy((void*)api_obj->key, (void*)keyText, size);
        api_obj->key_len = size;
        alchemy_trim_key(api_obj);
        ret = alchemy_check_key(api_obj) ? ALCHEMY_OK : ALCHEMY_ILLEGAL_KEY_SPECIFIED;
    }

    if (ALCHEMY_OK != ret)
        alchemy_free_key_util(api_obj);

    return ret;
}

Alchemy_code alchemy_load_key_util(alchemyAPI_obj* api_obj, const char* path)
{
    long size = 0L;
    FILE* file = NULL;
    Alchemy_code ret = ALCHEMY_LAST;

    if (NULL == api_obj)
        return ALCHEMY_ILLEGAL_KEY_SPECIFIED;
    if (alchemy_is_empty(path))
        return ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED;

    file = fopen(path, "rb");
    if (NULL == file)
        return ALCHEMY_UNABLE_TO_READ_KEY;

    if (-1 == fseek(file, 0, SEEK_END) || -1 == (size = ftell(file)))
    {
        fclose(file);
        return ALCHEMY_UNABLE_TO_READ_KEY;
    }
    rewind(file);

    ret = ALCHEMY_UNABLE_TO_READ_KEY;
    if (size > 0L)
    {
        api_obj->key = (char*)malloc(sizeof(char) * size);
        if (NULL != api_obj->key &&
            fread((void*)api_obj->key, sizeof(char), size, file) == size)
        {
            api_obj->key_len = size;
            alchemy_trim_key(api_obj);
            ret = alchemy_check_key(api_obj) ? ALCHEMY_OK : ALCHEMY_ILLEGAL_KEY_SPECIFIED;
        }
    }

    if (ALCHEMY_OK != ret)
        alchemy_free_key_util(api_obj);
    fclose (file);

    return ret;
}

Alchemy_code alchemy_set_api_host(alchemyAPI_obj* api_obj, const char* host) {
	long size = 0L;
    Alchemy_code ret = ALCHEMY_LAST;

    if (NULL == api_obj)
        return ALCHEMY_ILLEGAL_HOST_SPECIFIED;
    if (alchemy_is_empty(host))
        return ALCHEMY_ILLEGAL_HOST_SPECIFIED;

    size = strlen(host);
    if (size < 1)
        return ALCHEMY_ILLEGAL_HOST_SPECIFIED;

    api_obj->host = (char*)malloc(sizeof(char) * (size + 1));
    if (NULL != api_obj->host)
    {
        memcpy((void*)api_obj->host, (void*)host, (size + 1));
        api_obj->host_len = size;
        ret = alchemy_check_host(api_obj) ? ALCHEMY_OK : ALCHEMY_ILLEGAL_HOST_SPECIFIED;
    }

    if (ALCHEMY_OK != ret)
        alchemy_free_host_util(api_obj);

    return ret;
}	


Alchemy_code alchemy_set_custom_params(Alchemy_base_params* base_params, int8_t params_num, va_list temp_list) {
	char* escaped_value = NULL;
	int8_t i;
	size_t len = 0;
	CURL* curl_hndlr = curl_easy_init();
	if( NULL != base_params->custom_params ) {
		free( base_params->custom_params );
		base_params->custom_params = NULL;
	}
	len = 1u; // '\0'

	base_params->custom_params = malloc(len);
	memset(base_params->custom_params,0,len);
	
	for (i=params_num;i>0 ; --i)
	{
		const char* param = va_arg(temp_list, const char*);
        	const char* value = va_arg(temp_list, const char*);
		
		if (alchemy_is_empty(param)) 
			return -1;

		len += strlen(param) + 1;
		base_params->custom_params = realloc(base_params->custom_params, len);

		strcat(base_params->custom_params, "&");
		strcat(base_params->custom_params, param);

		if (NULL != value)
		{
			escaped_value = curl_easy_escape(curl_hndlr, value, 0);

			len += strlen(escaped_value) + 1;
			base_params->custom_params = realloc(base_params->custom_params, len);

			strcat(base_params->custom_params, "=");
			strcat(base_params->custom_params, escaped_value);

			curl_free(escaped_value);
		}
	}
	
	curl_easy_cleanup(curl_hndlr);
	va_end(temp_list);
	return ALCHEMY_OK;
}

Alchemy_code alchemy_url_get_author(const alchemyAPI_obj* api_obj, const char* url,
				  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetAuthor",
				"url", 1u, "url", url);
}

Alchemy_code alchemy_html_get_author(const alchemyAPI_obj* api_obj, const char* html, const char* url,
				  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "HTMLGetAuthor",
				"html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_url_get_author_params(const alchemyAPI_obj* api_obj, const char* url,
				   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};

    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url);

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetAuthor",
				"url", &super_params);
}


Alchemy_code alchemy_html_get_author_params(const alchemyAPI_obj* api_obj, const char* html, const char* url,
				   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};

    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html);
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url);

    return alchemy_get_request_parameters(api_obj, callback, data, "HTMLGetAuthor",
				"html", &super_params);
}

Alchemy_code alchemy_url_get_category(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetCategory",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_category_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};

    super_params.category_params = category_params;
    alchemy_category_param_set_string( super_params.category_params, ALCHEMY_PARAMETERS_URL, url);

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetCategory",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_category(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetCategory",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_category_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
	
    super_params.category_params = category_params;
    alchemy_category_param_set_string( super_params.category_params, ALCHEMY_PARAMETERS_HTML, html);
    alchemy_category_param_set_string( super_params.category_params, ALCHEMY_PARAMETERS_URL, url);

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetCategory",
                              "html", &super_params);
}

Alchemy_code alchemy_text_get_category(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetCategory",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_category_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)
	
    Alchemy_params super_params = {0u};
	
    super_params.category_params = category_params;
    alchemy_category_param_set_string( super_params.category_params, ALCHEMY_PARAMETERS_TEXT, text);

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetCategory",
                              "text", &super_params);
}

Alchemy_code alchemy_url_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRankedNamedEntities",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};
    super_params.entity_params = entity_params;
    alchemy_entity_param_set_string( super_params.entity_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRankedNamedEntities",
                                          "url", &super_params);
}

Alchemy_code alchemy_text_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetRankedNamedEntities",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    Alchemy_params super_params = {0u};
    super_params.entity_params = entity_params;
    alchemy_entity_param_set_string( super_params.entity_params, ALCHEMY_PARAMETERS_TEXT, text );

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetRankedNamedEntities",
                                           "text", &super_params);
}

Alchemy_code alchemy_html_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRankedNamedEntities",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    Alchemy_params super_params = {0u};
    super_params.entity_params = entity_params;
    alchemy_entity_param_set_string( super_params.entity_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_entity_param_set_string( super_params.entity_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRankedNamedEntities",
                                           "html", &super_params);
}

Alchemy_code alchemy_url_get_relations(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRelations",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};
    super_params.relation_params = relation_params;
    alchemy_base_param_set_string( &super_params.relation_params->base_params,
                                   ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRelations",
                                          "url", &super_params);
}

Alchemy_code alchemy_text_get_relations(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetRelations",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    Alchemy_params super_params = {0u};
    super_params.relation_params = relation_params;
    alchemy_base_param_set_string( &super_params.relation_params->base_params,
                                   ALCHEMY_PARAMETERS_TEXT, text );

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetRelations",
                                           "text", &super_params);
}

Alchemy_code alchemy_html_get_relations(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRelations",
                                "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    Alchemy_params super_params = {0u};
    super_params.relation_params = relation_params;
    alchemy_base_param_set_string( &super_params.relation_params->base_params,
                                    ALCHEMY_PARAMETERS_HTML, html );
    alchemy_base_param_set_string( &super_params.relation_params->base_params,
                                   ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRelations",
                                           "html", &super_params);
}

Alchemy_code alchemy_url_get_feed_links(const alchemyAPI_obj* api_obj, const char* url,
                                    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetFeedLinks",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_feed_links_params(const alchemyAPI_obj* api_obj, const char* url,
                                    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetFeedLinks",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_feed_links(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetFeedLinks",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_feed_links_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetFeedLinks",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_ranked_concepts(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRankedConcepts",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_ranked_concepts_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.concept_params = concept_params;
    alchemy_concept_param_set_string( super_params.concept_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRankedConcepts",
                              "url", &super_params );
}

Alchemy_code alchemy_text_get_ranked_concepts(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetRankedConcepts",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_ranked_concepts_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)
	
    Alchemy_params super_params = {0u};
    super_params.concept_params = concept_params;
    alchemy_concept_param_set_string( super_params.concept_params, ALCHEMY_PARAMETERS_TEXT, text );

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetRankedConcepts",
                              "text", &super_params);
}

Alchemy_code alchemy_html_get_ranked_concepts(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRankedConcepts",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_ranked_concepts_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.concept_params = concept_params;
    alchemy_concept_param_set_string( super_params.concept_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_concept_param_set_string( super_params.concept_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRankedConcepts",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_ranked_keywords(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRankedKeywords",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_ranked_keywords_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.keyword_params = keyword_params;
    alchemy_keyword_param_set_string( super_params.keyword_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRankedKeywords",
                              "url", &super_params );
}

Alchemy_code alchemy_text_get_ranked_keywords(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetRankedKeywords",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_ranked_keywords_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)
	
    Alchemy_params super_params = {0u};
    super_params.keyword_params = keyword_params;
    alchemy_keyword_param_set_string( super_params.keyword_params, ALCHEMY_PARAMETERS_TEXT, text );

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetRankedKeywords",
                              "text", &super_params);
}

Alchemy_code alchemy_html_get_ranked_keywords(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRankedKeywords",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_ranked_keywords_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.keyword_params = keyword_params;
    alchemy_keyword_param_set_string( super_params.keyword_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_keyword_param_set_string( super_params.keyword_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRankedKeywords",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_language(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetLanguage",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_language_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
	
    super_params.language_params = language_params;
    alchemy_language_param_set_string( super_params.language_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetLanguage",
                              "url", &super_params);
}


Alchemy_code alchemy_text_get_language(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetLanguage",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_language_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)
	
    Alchemy_params super_params = {0u};
	
    super_params.language_params = language_params;
    alchemy_language_param_set_string( super_params.language_params, ALCHEMY_PARAMETERS_TEXT, text );
	
    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetLanguage",
                              "text", &super_params);
}

Alchemy_code alchemy_html_get_language(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetLanguage",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_language_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
	
    super_params.language_params = language_params;
    alchemy_language_param_set_string( super_params.language_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_language_param_set_string( super_params.language_params, ALCHEMY_PARAMETERS_HTML, html );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetLanguage",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_microformats(const alchemyAPI_obj* api_obj, const char* url,
                                      alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetMicroformatData",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_microformats_params(const alchemyAPI_obj* api_obj, const char* url,
                                      alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetMicroformatData",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_microformats(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetMicroformatData",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_microformats_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetMicroformatData",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_textsentiment(const alchemyAPI_obj* api_obj, const char* url,
								alchemy_ret_callback callback, void* data)
{
	ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);
	
	return alchemy_get_request(api_obj, callback, data, "URLGetTextSentiment",
								"url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_textsentiment_params(const alchemyAPI_obj* api_obj, const char* url,
								alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
	ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);
	
	Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );
	
	return alchemy_get_request_parameters(api_obj, callback, data, "URLGetTextSentiment",
								"url", &super_params);
}

Alchemy_code alchemy_html_get_textsentiment(const alchemyAPI_obj* api_obj, const char* html, const char* url,
							alchemy_ret_callback callback, void* data)
{
	ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);
	if(!alchemy_check_html(html))
		return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
	
	return alchemy_post_request(api_obj, callback, data, "HTMLGetTextSentiment",
								"html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_textsentiment_params(const alchemyAPI_obj* api_obj, const char* html, const char* url,
							alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
	ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);
	if(!alchemy_check_html(html))
		return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

	Alchemy_params super_params = {0u};

    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetTextSentiment",
                              "html", &super_params);
}

Alchemy_code alchemy_text_get_textsentiment(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text);

    return alchemy_post_request(api_obj, callback, data, "TextGetTextSentiment",
                              "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_textsentiment_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text);
	
    Alchemy_params super_params = {0u};
	
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_TEXT, text);

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetTextSentiment",
                              "text", &super_params);
}

Alchemy_code alchemy_text_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* text, const char* target,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text);

    return alchemy_get_request(api_obj, callback, data, "TextGetTargetedSentiment",
                              "text", 2u, "text", text, "target", target);
}

Alchemy_code alchemy_html_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* target,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);

    return alchemy_post_request(api_obj, callback, data, "HTMLGetTargetedSentiment",
                              "html", 3u, "html", html, "url", url,"target", target);
}

Alchemy_code alchemy_url_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* url, const char* target,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);

    return alchemy_get_request(api_obj, callback, data, "URLGetTargetedSentiment",
                              "url", 2u, "url", url, "target", target);
}

Alchemy_code alchemy_text_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* text, const char* target,
				alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text);

    Alchemy_params super_params = {0u};

    super_params.targeted_sentiment_params = targeted_sentiment_params;
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_TEXT, text);
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_SENTIMENT_PHRASE, target);

    return alchemy_get_request_parameters(api_obj, callback, data, "TextGetTargetedSentiment",
				"text", &super_params);
}

Alchemy_code alchemy_html_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* target,
				alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);

    Alchemy_params super_params = {0u};

    super_params.targeted_sentiment_params = targeted_sentiment_params;
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_HTML, html);
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_URL, url);
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_SENTIMENT_PHRASE, target);

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetTargetedSentiment",
				"html", &super_params);
}

Alchemy_code alchemy_url_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* url, const char* target,
				alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url);

    Alchemy_params super_params = {0u};

    super_params.targeted_sentiment_params = targeted_sentiment_params;
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_URL, url);
    alchemy_targeted_sentiment_param_set_string( super_params.targeted_sentiment_params, ALCHEMY_PARAMETERS_SENTIMENT_PHRASE, target);

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetTargetedSentiment",
				"url", &super_params);
}

Alchemy_code alchemy_url_get_text(const alchemyAPI_obj* api_obj, const char* url,
                              alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetText",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_text_params(const alchemyAPI_obj* api_obj, const char* url,
                              alchemy_ret_callback callback, void* data, Alchemy_text_params* text_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};

    super_params.text_params = text_params;
    alchemy_text_param_set_string( super_params.text_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetText",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_text(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetText",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_text_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_text_params* text_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};

    super_params.text_params = text_params;
    alchemy_text_param_set_string( super_params.text_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_text_param_set_string( super_params.text_params, ALCHEMY_PARAMETERS_HTML, html );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetText",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_raw_text(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRawText",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_raw_text_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
	
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRawText",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_raw_text(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRawText",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_raw_text_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRawText",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_title(const alchemyAPI_obj* api_obj, const char* url,
                               alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetTitle",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_title_params(const alchemyAPI_obj* api_obj, const char* url,
                               alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetTitle",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_title(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetTitle",
                              "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_title_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.base_params = base_params;
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_base_param_set_string( super_params.base_params, ALCHEMY_PARAMETERS_URL, url );;

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetTitle",
                              "html", &super_params);
}

Alchemy_code alchemy_url_get_constraint_query(
    const alchemyAPI_obj* api_obj, const char* url, const char* query,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_constraint_query(query))
        return ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED;

    return alchemy_get_request(api_obj, callback, data, "URLGetConstraintQuery",
                              "url", 2u, "url", url, "cquery", query);
}

Alchemy_code alchemy_url_get_constraint_query_params(
    const alchemyAPI_obj* api_obj, const char* url, const char* query,
    alchemy_ret_callback callback, void* data, Alchemy_constraint_query_params* constraint_query_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_constraint_query(query))
        return ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED;
		
    Alchemy_params super_params = {0u};
    super_params.constraint_query_params = constraint_query_params;
    alchemy_constraint_query_param_set_string( super_params.constraint_query_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_constraint_query_param_set_string( super_params.constraint_query_params, ALCHEMY_PARAMETERS_CQUERY, query );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetConstraintQuery",
                              "url", &super_params);
}

Alchemy_code alchemy_html_get_constraint_query(
    const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* query,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
    if (!alchemy_check_constraint_query(query))
        return ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetConstraintQuery",
                              "html", 3u, "url", url, "html", html,
                                          "cquery", query);
}

Alchemy_code alchemy_html_get_constraint_query_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* query,
    alchemy_ret_callback callback, void* data, Alchemy_constraint_query_params* constraint_query_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;
    if (!alchemy_check_constraint_query(query))
        return ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED;
	
    Alchemy_params super_params = {0u};
    super_params.constraint_query_params = constraint_query_params;
    alchemy_constraint_query_param_set_string( super_params.constraint_query_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_constraint_query_param_set_string( super_params.constraint_query_params, ALCHEMY_PARAMETERS_HTML, html );
    alchemy_constraint_query_param_set_string( super_params.constraint_query_params, ALCHEMY_PARAMETERS_CQUERY, query );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetConstraintQuery",
                              "html", &super_params);
}


Alchemy_code alchemy_url_get_ranked_taxonomy(const alchemyAPI_obj* api_obj, const char* url,
                                             alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetRankedTaxonomy",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_taxonomy_params(const alchemyAPI_obj* api_obj, const char* url,
                                             alchemy_ret_callback callback, void* data, Alchemy_taxonomy_params* taxonomy_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};

    super_params.taxonomy_params = taxonomy_params;
    alchemy_taxonomy_param_set_string( super_params.taxonomy_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetRankedTaxonomy",
                              "url", &super_params);
}


Alchemy_code alchemy_text_get_ranked_taxonomy(const alchemyAPI_obj* api_obj, const char* text,
                                              alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    return alchemy_post_request(api_obj, callback, data, "TextGetRankedTaxonomy",
                                "text", 1u, "text", text);
}

Alchemy_code alchemy_text_get_taxonomy_params(const alchemyAPI_obj* api_obj, const char* text,
                                              alchemy_ret_callback callback, void* data, Alchemy_taxonomy_params* taxonomy_params)
{
    ALCHEMY_CHECK_TEXT_PARAMS(api_obj, callback, text)

    Alchemy_params super_params = {0u};

    super_params.taxonomy_params = taxonomy_params;
    alchemy_taxonomy_param_set_string( super_params.taxonomy_params, ALCHEMY_PARAMETERS_TEXT, text );

    return alchemy_post_request_parameters(api_obj, callback, data, "TextGetRankedTaxonomy",
                              "text", &super_params);
}

Alchemy_code alchemy_html_get_ranked_taxonomy(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetRankedTaxonomy",
                                "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_taxonomy_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_taxonomy_params* taxonomy_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    Alchemy_params super_params = {0u};

    super_params.taxonomy_params = taxonomy_params;
    alchemy_taxonomy_param_set_string( super_params.taxonomy_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_taxonomy_param_set_string( super_params.taxonomy_params, ALCHEMY_PARAMETERS_HTML, html );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetRankedTaxonomy",
                                           "html", &super_params);
}



Alchemy_code alchemy_url_get_image(const alchemyAPI_obj* api_obj, const char* url,
                                   alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetImage",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_image_params(const alchemyAPI_obj* api_obj, const char* url,
                                          alchemy_ret_callback callback, void* data,
                                          Alchemy_image_params* image_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};

    super_params.image_params = image_params;
    alchemy_image_param_set_string( super_params.image_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetImage",
                                          "url", &super_params);
}

Alchemy_code alchemy_html_get_image(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    return alchemy_post_request(api_obj, callback, data, "HTMLGetImage",
                                "html", 2u, "html", html, "url", url);
}

Alchemy_code alchemy_html_get_image_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_image_params* image_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)
    if (!alchemy_check_html(html))
        return ALCHEMY_ILLEGAL_HTML_SPECIFIED;

    Alchemy_params super_params = {0u};

    super_params.image_params = image_params;
    alchemy_image_param_set_string( super_params.image_params, ALCHEMY_PARAMETERS_URL, url );
    alchemy_image_param_set_string( super_params.image_params, ALCHEMY_PARAMETERS_HTML, html );

    return alchemy_post_request_parameters(api_obj, callback, data, "HTMLGetImage",
                                           "html", &super_params);
}


Alchemy_code alchemy_url_get_combined_data(const alchemyAPI_obj* api_obj, const char* url,
                                           alchemy_ret_callback callback, void* data)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    return alchemy_get_request(api_obj, callback, data, "URLGetCombinedData",
                              "url", 1u, "url", url);
}

Alchemy_code alchemy_url_get_combined_data_params(const alchemyAPI_obj* api_obj, const char* url,
                                                  alchemy_ret_callback callback, void* data,
                                                  Alchemy_combined_data_params* combined_data_params)
{
    ALCHEMY_CHECK_URL_PARAMS(api_obj, callback, url)

    Alchemy_params super_params = {0u};

    super_params.combined_data_params = combined_data_params;
    alchemy_combined_data_param_set_string( super_params.combined_data_params, ALCHEMY_PARAMETERS_URL, url );

    return alchemy_get_request_parameters(api_obj, callback, data, "URLGetCombinedData",
                                          "url", &super_params);
}


static char* alchemy_copy_strings(char* param_string, const char* value )
{
	size_t len = strlen(value);
	len += 1u; // '\0'
	if( param_string == NULL ) {
		param_string = (char*)malloc(sizeof(char)*len);
	}
	else {
		param_string = (char*)realloc((void*)param_string,sizeof(char)*len);
	}
	strcpy(param_string,value);
	return param_string;
}


Alchemy_code alchemy_concept_param_set_string(Alchemy_concept_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_concept_param_set_int(Alchemy_concept_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_MAX_RETRIEVE:
			param_struct->max_retrieve = value;
			break;
		case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
			param_struct->show_source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
		case ALCHEMY_PARAMETERS_LINKED_DATA:
			param_struct->linked_data = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_concept_param_set_custom_params(Alchemy_concept_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_concept_params alchemy_concept_param_init()
{
	Alchemy_concept_params param_struct = {0u};
	param_struct.base_params.url = NULL;
	param_struct.base_params.text = NULL;
	param_struct.base_params.html = NULL;
	param_struct.base_params.custom_params = NULL;
	return param_struct;
}

Alchemy_code alchemy_concept_param_free(Alchemy_concept_params* param_struct)
{
	if (NULL != param_struct)
	{
		if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;

	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_keyword_param_set_string(Alchemy_keyword_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;
		case ALCHEMY_PARAMETERS_BASE_URL:
			param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_keyword_param_set_int(Alchemy_keyword_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_MAX_RETRIEVE:
			param_struct->max_retrieve = value;
			break;
		case ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE:
			param_struct->keyword_extract_mode = value;
			break;
		case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
			param_struct->show_source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SENTIMENT:
			param_struct->sentiment = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_keyword_param_set_custom_params(Alchemy_keyword_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_keyword_params alchemy_keyword_param_init()
{
	Alchemy_keyword_params param_struct = {0u};
	param_struct.base_params.url = NULL;
	param_struct.base_params.text = NULL;
	param_struct.base_params.html = NULL;
	param_struct.base_params.custom_params = NULL;
	return param_struct;
}

Alchemy_code alchemy_keyword_param_free(Alchemy_keyword_params* param_struct)
{
	if (NULL != param_struct)
	{
		if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
		if (NULL != param_struct->base_url) {
	            free((void*)param_struct->base_url);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;
		param_struct->base_url = NULL;

	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_category_param_set_string(Alchemy_category_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;
		case ALCHEMY_PARAMETERS_BASE_URL:
			param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_category_param_set_int(Alchemy_category_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_category_param_set_custom_params(Alchemy_category_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_category_params alchemy_category_param_init()
{
	Alchemy_category_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_category_param_free(Alchemy_category_params* param_struct)
{
	if (NULL != param_struct)
	{
        if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
		if (NULL != param_struct->base_url) {
	            free((void*)param_struct->base_url);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;
		param_struct->base_url = NULL;
	}
	return ALCHEMY_OK;
}



Alchemy_code alchemy_entity_param_set_string(Alchemy_entity_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;
		case ALCHEMY_PARAMETERS_BASE_URL:
			param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_entity_param_set_int(Alchemy_entity_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_MAX_RETRIEVE:
			param_struct->max_retrieve = value;
			break;
		case ALCHEMY_PARAMETERS_DISAMBIGUATE:
			param_struct->disambiguate = value;
			break;
		case ALCHEMY_PARAMETERS_LINKED_DATA:
			param_struct->linked_data = value;
			break;
		case ALCHEMY_PARAMETERS_COREFERENCE:
			param_struct->coreference = value;
			break;
		case ALCHEMY_PARAMETERS_QUOTATIONS:
			param_struct->quotations = value;
			break;
		case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
			param_struct->show_source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SENTIMENT:
			param_struct->sentiment = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_entity_param_set_custom_params(Alchemy_entity_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_entity_params alchemy_entity_param_init()
{
	Alchemy_entity_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_entity_param_free(Alchemy_entity_params* param_struct)
{
	if (NULL != param_struct)
	{
		if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
		if (NULL != param_struct->base_url) {
	            free((void*)param_struct->base_url);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;
		param_struct->base_url = NULL;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_relation_param_set_string(Alchemy_relation_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;
		case ALCHEMY_PARAMETERS_BASE_URL:
			param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_relation_param_set_int(Alchemy_relation_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_MAX_RETRIEVE:
			param_struct->max_retrieve = value;
			break;
		case ALCHEMY_PARAMETERS_DISAMBIGUATE:
			param_struct->disambiguate = value;
			break;
		case ALCHEMY_PARAMETERS_LINKED_DATA:
			param_struct->linked_data = value;
			break;
		case ALCHEMY_PARAMETERS_COREFERENCE:
			param_struct->coreference = value;
			break;
		case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
			param_struct->show_source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
		case ALCHEMY_PARAMETERS_SENTIMENT:
			param_struct->sentiment = value;
			break;
		case ALCHEMY_PARAMETERS_ENTITIES:
			param_struct->entities = value;
			break;
		case ALCHEMY_PARAMETERS_REQUIRED_ENTITIES:
			param_struct->required_entities = value;
			break;
		case ALCHEMY_PARAMETERS_EXCLUDE_SENTI_ENTITIES:
			param_struct->sentiment_exclude_entities = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_relation_params alchemy_relation_param_init()
{
	Alchemy_relation_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_relation_param_free(Alchemy_relation_params* param_struct)
{
	if (NULL != param_struct)
	{
		if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
		if (NULL != param_struct->base_url) {
	            free((void*)param_struct->base_url);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;
		param_struct->base_url = NULL;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_base_param_set_string(Alchemy_base_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->url = alchemy_copy_strings(param_struct->url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->html = alchemy_copy_strings(param_struct->html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->text = alchemy_copy_strings(param_struct->text, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_base_param_set_int(Alchemy_base_params* param_struct, Alchemy_parameter_names param, int value)
{
	return ALCHEMY_OK;
}

Alchemy_code alchemy_base_param_set_custom_params(Alchemy_base_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( param_struct, params_num, temp_list);
}

Alchemy_base_params alchemy_base_param_init()
{
	Alchemy_base_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_base_param_free(Alchemy_base_params* param_struct)
{
	if (NULL != param_struct)
	{
	        if (NULL != param_struct->url) {
	            free((void*)param_struct->url);
		}
		if (NULL != param_struct->html) {
	            free((void*)param_struct->html);
		}
		if (NULL != param_struct->text) {
	            free((void*)param_struct->text);
		}
		if (NULL != param_struct->custom_params) {
	            free((void*)param_struct->custom_params);
		}
			
		param_struct->url = NULL;
		param_struct->html = NULL;
		param_struct->text = NULL;
		param_struct->custom_params = NULL;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_language_param_set_string(Alchemy_language_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
		case ALCHEMY_PARAMETERS_XPATH:
			param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
			break;

	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_language_param_set_int(Alchemy_language_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_SOURCE_TEXT:
			param_struct->source_text = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_language_param_set_custom_params(Alchemy_language_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_language_params alchemy_language_param_init()
{
	Alchemy_language_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_language_param_free(Alchemy_language_params* param_struct)
{
	if (NULL != param_struct)
	{
	        if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
		if (NULL != param_struct->xpath) {
	            free((void*)param_struct->xpath);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
		param_struct->cquery = NULL;
		param_struct->xpath = NULL;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_text_param_set_string(Alchemy_text_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_text_param_set_int(Alchemy_text_params* param_struct, Alchemy_parameter_names param, int value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_USE_METADATA:
			param_struct->use_metadata = value;
			break;
		case ALCHEMY_PARAMETERS_EXTRACT_LINKS:
			param_struct->extract_links = value;
			break;
	}

	return ALCHEMY_OK;
}

Alchemy_code alchemy_text_param_set_custom_params(Alchemy_text_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_text_params alchemy_text_param_init()
{
	Alchemy_text_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_text_param_free(Alchemy_text_params* param_struct)
{
	if (NULL != param_struct)
	{
		if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
	}
	return ALCHEMY_OK;
}


Alchemy_code alchemy_constraint_query_param_set_string(Alchemy_constraint_query_params* param_struct, Alchemy_parameter_names param, const char* value)
{
	switch( param ) {
		case ALCHEMY_PARAMETERS_URL:
			param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
			break;
		case ALCHEMY_PARAMETERS_HTML:
			param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
			break;
		case ALCHEMY_PARAMETERS_TEXT:
			param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
			break;
		case ALCHEMY_PARAMETERS_CQUERY:
			param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
			break;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_constraint_query_param_set_int(Alchemy_constraint_query_params* param_struct, Alchemy_parameter_names param, int value)
{

	return ALCHEMY_OK;
}

Alchemy_code alchemy_constraint_query_param_set_custom_params(Alchemy_constraint_query_params* param_struct, int8_t params_num,...)
{
	va_list temp_list;
	va_start(temp_list, params_num);
	
	return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_constraint_query_params alchemy_constraint_query_param_init()
{
	Alchemy_constraint_query_params param_struct = {0u};
	return param_struct;
}

Alchemy_code alchemy_constraint_query_param_free(Alchemy_constraint_query_params* param_struct)
{
	if (NULL != param_struct)
	{
        	if (NULL != param_struct->base_params.url) {
	            free((void*)param_struct->base_params.url);
		}
		if (NULL != param_struct->base_params.html) {
	            free((void*)param_struct->base_params.html);
		}
		if (NULL != param_struct->base_params.text) {
	            free((void*)param_struct->base_params.text);
		}
		if (NULL != param_struct->base_params.custom_params) {
	            free((void*)param_struct->base_params.custom_params);
		}
		if (NULL != param_struct->cquery) {
	            free((void*)param_struct->cquery);
		}
			
		param_struct->base_params.url = NULL;
		param_struct->base_params.html = NULL;
		param_struct->base_params.text = NULL;
		param_struct->base_params.custom_params = NULL;
		param_struct->cquery = NULL;
	}
	return ALCHEMY_OK;
}

Alchemy_code alchemy_targeted_sentiment_param_set_string(Alchemy_targeted_sentiment_params* param_struct, Alchemy_parameter_names param, const char* value)
{
        switch( param ) {
                case ALCHEMY_PARAMETERS_TEXT:
                        param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
                        break;
                case ALCHEMY_PARAMETERS_HTML:
                        param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
                        break;
                case ALCHEMY_PARAMETERS_URL:
                        param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
                        break;
                case ALCHEMY_PARAMETERS_SENTIMENT_PHRASE:
                        param_struct->sentiment_phrase = alchemy_copy_strings(param_struct->sentiment_phrase, value);
                        break;
        }
        return ALCHEMY_OK;
}

Alchemy_code alchemy_targeted_sentiment_param_set_int(Alchemy_targeted_sentiment_params* param_struct, Alchemy_parameter_names param, int value)
{
        switch( param ) {
                case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
                        param_struct->show_source_text = value;
                        break;
        }

        return ALCHEMY_OK;
}

Alchemy_code alchemy_targeted_sentiment_param_set_custom_params(Alchemy_targeted_sentiment_params* param_struct, int8_t params_num,...)
{
        va_list temp_list;
        va_start(temp_list, params_num);

        return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_targeted_sentiment_params alchemy_targeted_sentiment_param_init()
{
        Alchemy_targeted_sentiment_params param_struct = {0u};
        //param_struct.base_params.text = NULL;
        //param_struct.base_params.html = NULL;
        //param_struct.base_params.url = NULL;
        //param_struct.base_params.custom_params = NULL;
        return param_struct;
}

Alchemy_code alchemy_targeted_sentiment_param_free(Alchemy_targeted_sentiment_params* param_struct)
{
        if (NULL != param_struct)
        {
                if (NULL != param_struct->base_params.text) {
                    free((void*)param_struct->base_params.text);
                }
                if (NULL != param_struct->base_params.html) {
                    free((void*)param_struct->base_params.html);
                }
                if (NULL != param_struct->base_params.url) {
                    free((void*)param_struct->base_params.url);
                }
                if (NULL != param_struct->base_params.custom_params) {
                    free((void*)param_struct->base_params.custom_params);
                }
                if (NULL != param_struct->sentiment_phrase) {
                    free((void*)param_struct->sentiment_phrase);
                }

                param_struct->base_params.text = NULL;
                param_struct->base_params.url = NULL;
                param_struct->base_params.html = NULL;
                param_struct->base_params.custom_params = NULL;
                param_struct->sentiment_phrase = NULL;

        }
        return ALCHEMY_OK;
}


Alchemy_code alchemy_taxonomy_param_set_string(Alchemy_taxonomy_params* param_struct,
                                               Alchemy_parameter_names param,
                                               const char* value)
{
    switch( param ) {
        case ALCHEMY_PARAMETERS_URL:
            param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
            break;
        case ALCHEMY_PARAMETERS_HTML:
            param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
            break;
        case ALCHEMY_PARAMETERS_TEXT:
            param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
            break;
        case ALCHEMY_PARAMETERS_CQUERY:
            param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
            break;
        case ALCHEMY_PARAMETERS_XPATH:
            param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
            break;
        case ALCHEMY_PARAMETERS_BASE_URL:
            param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
            break;
    }
    return ALCHEMY_OK;
}

Alchemy_code alchemy_taxonomy_param_set_int(Alchemy_taxonomy_params* param_struct,
                                            Alchemy_parameter_names param, int value)
{
    switch( param ) {
        case ALCHEMY_PARAMETERS_SOURCE_TEXT:
            param_struct->source_text = value;
            break;
    }

    return ALCHEMY_OK;
}

Alchemy_code alchemy_taxonomy_param_set_custom_params(Alchemy_taxonomy_params* param_struct,
                                                      int8_t params_num,...)
{
    va_list temp_list;
    va_start(temp_list, params_num);

    return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_taxonomy_params alchemy_taxonomy_param_init()
{
    Alchemy_taxonomy_params param_struct = {0u};
    param_struct.base_params.url = NULL;
    param_struct.base_params.text = NULL;
    param_struct.base_params.html = NULL;
    param_struct.base_params.custom_params = NULL;
    param_struct.cquery = NULL;
    param_struct.xpath = NULL;
    param_struct.base_url = NULL;
    return param_struct;
}

Alchemy_code alchemy_taxonomy_param_free(Alchemy_taxonomy_params* param_struct)
{
    if (NULL != param_struct)
    {
        if (NULL != param_struct->base_params.url) {
                free((void*)param_struct->base_params.url);
        }
        if (NULL != param_struct->base_params.html) {
                free((void*)param_struct->base_params.html);
        }
        if (NULL != param_struct->base_params.text) {
                free((void*)param_struct->base_params.text);
        }
        if (NULL != param_struct->base_params.custom_params) {
                free((void*)param_struct->base_params.custom_params);
        }
        if (NULL != param_struct->cquery) {
                free((void*)param_struct->cquery);
        }
        if (NULL != param_struct->xpath) {
                free((void*)param_struct->xpath);
        }
        if (NULL != param_struct->base_url) {
                free((void*)param_struct->base_url);
        }

        param_struct->base_params.url = NULL;
        param_struct->base_params.html = NULL;
        param_struct->base_params.text = NULL;
        param_struct->base_params.custom_params = NULL;
        param_struct->cquery = NULL;
        param_struct->xpath = NULL;
        param_struct->base_url = NULL;
    }
    return ALCHEMY_OK;
}


Alchemy_code alchemy_image_param_set_string(Alchemy_image_params* param_struct,
                                            Alchemy_parameter_names param,
                                            const char* value)
{
        switch( param ) {
                case ALCHEMY_PARAMETERS_TEXT:
                        param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
                        break;
                case ALCHEMY_PARAMETERS_HTML:
                        param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
                        break;
                case ALCHEMY_PARAMETERS_URL:
                        param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
                        break;
        }
        return ALCHEMY_OK;
}

Alchemy_code alchemy_image_param_set_int(Alchemy_image_params* param_struct,
                                         Alchemy_parameter_names param, int value)
{
        switch( param ) {
                case ALCHEMY_PARAMETERS_IMAGE_EXTRACT_MODE:
                        param_struct->image_extract_mode = value;
                        break;
        }

        return ALCHEMY_OK;
}

Alchemy_code alchemy_image_param_set_custom_params(Alchemy_image_params* param_struct,
                                                   int8_t params_num,...)
{
        va_list temp_list;
        va_start(temp_list, params_num);

        return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_image_params alchemy_image_param_init()
{
        Alchemy_image_params param_struct = {0u};
        param_struct.base_params.url = NULL;
        param_struct.base_params.text = NULL;
        param_struct.base_params.html = NULL;
        param_struct.base_params.custom_params = NULL;
        return param_struct;
}

Alchemy_code alchemy_image_param_free(Alchemy_image_params* param_struct)
{
        if (NULL != param_struct)
        {
                if (NULL != param_struct->base_params.text) {
                    free((void*)param_struct->base_params.text);
                }
                if (NULL != param_struct->base_params.html) {
                    free((void*)param_struct->base_params.html);
                }
                if (NULL != param_struct->base_params.url) {
                    free((void*)param_struct->base_params.url);
                }
                if (NULL != param_struct->base_params.custom_params) {
                    free((void*)param_struct->base_params.custom_params);
                }

                param_struct->base_params.text = NULL;
                param_struct->base_params.url = NULL;
                param_struct->base_params.html = NULL;
                param_struct->base_params.custom_params = NULL;
        }
        return ALCHEMY_OK;
}



Alchemy_code alchemy_combined_data_param_set_string(Alchemy_combined_data_params* param_struct,
                                                    Alchemy_parameter_names param, const char* value)
{
    switch( param ) {
        case ALCHEMY_PARAMETERS_URL:
            param_struct->base_params.url = alchemy_copy_strings(param_struct->base_params.url, value);
            break;
        case ALCHEMY_PARAMETERS_HTML:
            param_struct->base_params.html = alchemy_copy_strings(param_struct->base_params.html, value);
            break;
        case ALCHEMY_PARAMETERS_TEXT:
            param_struct->base_params.text = alchemy_copy_strings(param_struct->base_params.text, value);
            break;
        case ALCHEMY_PARAMETERS_CQUERY:
            param_struct->cquery = alchemy_copy_strings(param_struct->cquery, value);
            break;
        case ALCHEMY_PARAMETERS_XPATH:
            param_struct->xpath = alchemy_copy_strings(param_struct->xpath, value);
            break;
        case ALCHEMY_PARAMETERS_BASE_URL:
            param_struct->base_url = alchemy_copy_strings(param_struct->base_url, value);
            break;
        case ALCHEMY_PARAMETERS_SENTIMENT_PHRASE:
            param_struct->sentiment_phrase = alchemy_copy_strings(param_struct->sentiment_phrase, value);
            break;
        case ALCHEMY_PARAMETERS_EXTRACT:
            param_struct->extract = alchemy_copy_strings(param_struct->extract, value);
            break;
    }
    return ALCHEMY_OK;
}

Alchemy_code alchemy_combined_data_param_set_int(Alchemy_combined_data_params* param_struct,
                                                 Alchemy_parameter_names param, int value)
{
    switch( param ) {
        case ALCHEMY_PARAMETERS_MAX_RETRIEVE:
            param_struct->max_retrieve = value;
            break;
        case ALCHEMY_PARAMETERS_DISAMBIGUATE:
            param_struct->disambiguate = value;
            break;
        case ALCHEMY_PARAMETERS_LINKED_DATA:
            param_struct->linked_data = value;
            break;
        case ALCHEMY_PARAMETERS_COREFERENCE:
            param_struct->coreference = value;
            break;
        case ALCHEMY_PARAMETERS_QUOTATIONS:
            param_struct->quotations = value;
            break;
        case ALCHEMY_PARAMETERS_SOURCE_TEXT:
            param_struct->source_text = value;
            break;
        case ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT:
            param_struct->show_source_text = value;
            break;
        case ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE:
            param_struct->keyword_extract_mode = value;
            break;
        case ALCHEMY_PARAMETERS_USE_METADATA:
            param_struct->use_metadata = value;
            break;
        case ALCHEMY_PARAMETERS_EXTRACT_LINKS:
            param_struct->extract_links = value;
            break;
        case ALCHEMY_PARAMETERS_SENTIMENT:
            param_struct->sentiment = value;
            break;
        case ALCHEMY_PARAMETERS_ENTITIES:
            param_struct->entities = value;
            break;
        case ALCHEMY_PARAMETERS_REQUIRED_ENTITIES:
            param_struct->required_entities = value;
            break;
        case ALCHEMY_PARAMETERS_EXCLUDE_SENTI_ENTITIES:
            param_struct->sentiment_exclude_entities = value;
            break;
        case ALCHEMY_PARAMETERS_IMAGE_EXTRACT_MODE:
            param_struct->image_extract_mode = value;
            break;
    }

    return ALCHEMY_OK;
}

Alchemy_code alchemy_combined_data_param_set_custom_params(Alchemy_combined_data_params* param_struct,
                                                           int8_t params_num,...)
{
    va_list temp_list;
    va_start(temp_list, params_num);

    return alchemy_set_custom_params( &param_struct->base_params, params_num, temp_list);
}

Alchemy_combined_data_params alchemy_combined_data_param_init()
{
    Alchemy_combined_data_params param_struct = {0u};
    param_struct.base_params.url = NULL;
    param_struct.base_params.text = NULL;
    param_struct.base_params.html = NULL;
    param_struct.base_params.custom_params = NULL;
    param_struct.cquery = NULL;
    param_struct.xpath = NULL;
    param_struct.base_url = NULL;
    param_struct.sentiment_phrase = NULL;
    return param_struct;
}

Alchemy_code alchemy_combined_data_param_free(Alchemy_combined_data_params* param_struct)
{
    if (NULL != param_struct)
    {
        if (NULL != param_struct->base_params.url) {
                free((void*)param_struct->base_params.url);
        }
        if (NULL != param_struct->base_params.html) {
                free((void*)param_struct->base_params.html);
        }
        if (NULL != param_struct->base_params.text) {
                free((void*)param_struct->base_params.text);
        }
        if (NULL != param_struct->base_params.custom_params) {
                free((void*)param_struct->base_params.custom_params);
        }
        if (NULL != param_struct->cquery) {
                free((void*)param_struct->cquery);
        }
        if (NULL != param_struct->xpath) {
                free((void*)param_struct->xpath);
        }
        if (NULL != param_struct->base_url) {
                free((void*)param_struct->base_url);
        }
        if (NULL != param_struct->sentiment_phrase) {
                free((void*)param_struct->sentiment_phrase);
        }

        param_struct->base_params.url = NULL;
        param_struct->base_params.html = NULL;
        param_struct->base_params.text = NULL;
        param_struct->base_params.custom_params = NULL;
        param_struct->cquery = NULL;
        param_struct->xpath = NULL;
        param_struct->base_url = NULL;
        param_struct->sentiment_phrase = NULL;
    }
    return ALCHEMY_OK;
}
