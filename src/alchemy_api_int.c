/*************************************************************************
 * Summary: AlchemyAPI
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#include <string.h>
#include <stdio.h>
#include <libxml/xpath.h>
#include <libxml/parser.h>

#include "alchemy_api_int.h"


typedef struct
{
    char* uri;
    char* data;

    va_list params;
    uint8_t paramsNum;

    char* ret_data;
    size_t ret_size;

    CURL* curl_hndlr;
} alchemy_post;


int alchemy_is_empty(const char* str)
{
    return (NULL == str || '\0' == *str);
}

static int alchemy_add_param(alchemy_post* post_data, const char* param,
                                                  const char* value)
{
    char* escaped_value = NULL;
    size_t len = strlen(post_data->data);
	
    if (alchemy_is_empty(param)) 
        return -1;

    len += 2u; // '&', '\0'
    len += strlen(param);
    if (!alchemy_is_empty(value))
    {

        escaped_value = curl_easy_escape(post_data->curl_hndlr, value, 0);

        if (NULL == escaped_value) 
            return -1;

        len += 1u + strlen(escaped_value); // '=' and the value
    }
    post_data->data = realloc((void*)post_data->data, len);

    strcat(post_data->data, "&");
    strcat(post_data->data, param);
    if (NULL != escaped_value)
    {
        strcat(post_data->data, "=");
        strcat(post_data->data, escaped_value);

        curl_free(escaped_value);
    }
    return 0;
}

static Alchemy_code alchemy_free(alchemy_post* post_data)
{
    if (NULL != post_data)
    {
        if (NULL != post_data->curl_hndlr)
            curl_easy_cleanup(post_data->curl_hndlr);
        if (NULL != post_data->uri)
            free(post_data->uri);
        if (NULL != post_data->data)
            free(post_data->data);
        if (NULL != post_data->ret_data)
            free(post_data->ret_data);

        va_end(post_data->params);
    }
}

static void alchemy_build_uri(alchemy_post* post_data, const alchemyAPI_obj* api_obj,
                            const char* prefix, const char* method)
{
    size_t len = strlen("http://");
    
    if(NULL != api_obj && NULL != api_obj->host && api_obj->host_len >= 5u) 
        len += api_obj->host_len;
    else 
        len += strlen("access");

    len += strlen(".alchemyapi.com/calls/");

    if (!alchemy_is_empty(prefix))
        len += strlen(prefix) + 1; /* +1 == "/" */
    if (!alchemy_is_empty(method))
        len += strlen(method);

    len += 1; // null terminator

    post_data->uri = (char*)malloc(len * sizeof(char));
    strcpy(post_data->uri, "http://");
    if(NULL != api_obj && NULL != api_obj->host && api_obj->host_len >= 5u) 
        strcat(post_data->uri, api_obj->host);
    else 
        strcat(post_data->uri, "access");

    strcat(post_data->uri, ".alchemyapi.com/calls/"); 
    if (!alchemy_is_empty(prefix))
    {
        strcat(post_data->uri, prefix);
        strcat(post_data->uri, "/");
    }
    if (!alchemy_is_empty(method))
        strcat(post_data->uri, method);
}

static Alchemy_code alchemy_prepare_request(
    alchemy_post* post_data, const alchemyAPI_obj* api_obj,
    const char* method, const char* prefix)
{
    post_data->curl_hndlr = curl_easy_init();
    if (NULL == post_data->curl_hndlr) {
        return ALCHEMY_REQUEST_INIT_FAILED;
	}

    post_data->data = (char*)malloc((api_obj->key_len + strlen("apikey=&outputMode=xml") + 1u) * sizeof(char));
    strcpy(post_data->data, "apikey=");
    strncat(post_data->data, api_obj->key, api_obj->key_len);
    strcat(post_data->data, "&outputMode=xml");
    for (; post_data->paramsNum > 0u; --(post_data->paramsNum))
    {
        const char* param = va_arg(post_data->params, const char*);
        const char* value = va_arg(post_data->params, const char*);
        if (-1 == alchemy_add_param(post_data, param, value))
            return ALCHEMY_INTERNAL_ERROR;
    }

    alchemy_build_uri(post_data, api_obj, prefix, method);

    return ALCHEMY_OK;
}

static Alchemy_code alchemy_base_param_struct_handler(alchemy_post* post_data, Alchemy_base_params params )
{
	char* escaped_value = NULL;
    size_t len = 0;

	if( params.url != NULL ) {
		if (-1 == alchemy_add_param(post_data, "url", params.url))
			return ALCHEMY_INTERNAL_ERROR;
	}
	if( params.text != NULL ) {
		if (-1 == alchemy_add_param(post_data, "text", params.text))
			return ALCHEMY_INTERNAL_ERROR;
	}
	if( params.html != NULL ) {
		if (-1 == alchemy_add_param(post_data, "html", params.html))
			return ALCHEMY_INTERNAL_ERROR;
	}
	len = strlen(post_data->data);
	if (params.custom_params != NULL) 
	{
		len += 1u; //  '\0'
		len += strlen(params.custom_params);
		post_data->data = realloc((void*)post_data->data, len);
		strcat(post_data->data, params.custom_params);
	}
	return ALCHEMY_OK;
}

static Alchemy_code alchemy_param_struct_handler(alchemy_post* post_data, Alchemy_params* params ) 
{
	int helper;
	char* temp_string;
	size_t len;
	if( params->keyword_params != NULL ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->keyword_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->keyword_params->show_source_text != 0 ) {
			switch( params->keyword_params->show_source_text )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->keyword_params->sentiment != 0 ) {
			switch( params->keyword_params->sentiment )
				{
						case ALCHEMY_BOOL_FALSE:
								if (-1 == alchemy_add_param(post_data, "sentiment", "0"))
										return ALCHEMY_INTERNAL_ERROR;
								break;
						case ALCHEMY_BOOL_TRUE:
								if (-1 == alchemy_add_param(post_data, "sentiment", "1"))
										return ALCHEMY_INTERNAL_ERROR;
								break;
				}
		}
		if( params->keyword_params->source_text != 0 ) {
			switch( params->keyword_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CLEANED:	
					if (-1 == alchemy_add_param(post_data, "sourcetext", "cleaned"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;				
			}
		}
		if( params->keyword_params->keyword_extract_mode != 0 ) {
			switch( params->keyword_params->keyword_extract_mode )
			{
				case ALCHEMY_NORMAL:	
					if (-1 == alchemy_add_param(post_data, "keywordExtractMode", "normal"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_STRICT:	
					if (-1 == alchemy_add_param(post_data, "keywordExtractMode", "strict"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->keyword_params->max_retrieve != 0 ) {
			char temp_string[16] = {0};
			sprintf( temp_string, "%i", params->keyword_params->max_retrieve );
			if (-1 == alchemy_add_param(post_data, "maxRetrieve", temp_string)) {
				return ALCHEMY_INTERNAL_ERROR;
			}	
		}
		if( params->keyword_params->cquery != NULL ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->keyword_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->keyword_params->xpath != NULL ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->keyword_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->keyword_params->base_url != NULL ) {
			if (-1 == alchemy_add_param(post_data, "baseUrl", params->keyword_params->base_url))
				return ALCHEMY_INTERNAL_ERROR;
		}
	}
	else if( params->concept_params != NULL ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->concept_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->concept_params->show_source_text != 0 ) {
			switch( params->concept_params->show_source_text )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->concept_params->source_text != 0 ) {
			switch( params->concept_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CLEANED:	
					if (-1 == alchemy_add_param(post_data, "sourcetext", "cleaned"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;				
			}
		}
		if( params->concept_params->max_retrieve != 0 ) {
			char temp_string[16] = {0};
			sprintf( temp_string, "%i", params->concept_params->max_retrieve );
			if (-1 == alchemy_add_param(post_data, "maxRetrieve", temp_string)) {
				return ALCHEMY_INTERNAL_ERROR;
			}	
		}
		if( params->concept_params->linked_data != 0 ) {
			switch( params->concept_params->linked_data )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->concept_params->cquery != NULL ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->concept_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->concept_params->xpath != NULL ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->concept_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
	}
	else if( params->entity_params != NULL ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->entity_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->entity_params->max_retrieve != 0 ) {
			char temp_string[16] = {0};
			sprintf( temp_string, "%i", params->entity_params->max_retrieve );
			if (-1 == alchemy_add_param(post_data, "maxRetrieve", temp_string)) {
				return ALCHEMY_INTERNAL_ERROR;
			}
		}
		if( params->entity_params->disambiguate != 0 ) {
			switch( params->entity_params->disambiguate )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "disambiguate", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "disambiguate", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->entity_params->linked_data != 0 ) {
			switch( params->entity_params->linked_data )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->entity_params->coreference != 0 ) {
			switch( params->entity_params->coreference )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "coreference", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "coreference", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->entity_params->quotations != 0 ) {
			switch( params->entity_params->quotations )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "quotations", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "quotations", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->entity_params->show_source_text != 0 ) {
			switch( params->entity_params->show_source_text )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->entity_params->source_text != 0 ) {
			switch( params->entity_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CLEANED:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;				
			}
		}
		if( params->entity_params->cquery != 0 ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->entity_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->entity_params->xpath != 0 ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->entity_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->entity_params->base_url != 0 ) {
			if (-1 == alchemy_add_param(post_data, "baseUrl", params->entity_params->base_url))
				return ALCHEMY_INTERNAL_ERROR;
		}	
	
 		if( params->entity_params->sentiment != 0 ) {
                	switch( params->entity_params->sentiment )
                        {
                                case ALCHEMY_BOOL_FALSE:
                                        if (-1 == alchemy_add_param(post_data, "sentiment", "0"))
                                                return ALCHEMY_INTERNAL_ERROR;
                                        break;
                                case ALCHEMY_BOOL_TRUE:
                                        if (-1 == alchemy_add_param(post_data, "sentiment", "1"))
                                                return ALCHEMY_INTERNAL_ERROR;
                                        break;
                        }
                }
	}
	else if( params->relation_params != NULL ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->relation_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->relation_params->max_retrieve != 0 ) {
			char temp_string[16] = {0};
			sprintf( temp_string, "%i", params->relation_params->max_retrieve );
			if (-1 == alchemy_add_param(post_data, "maxRetrieve", temp_string)) {
				return ALCHEMY_INTERNAL_ERROR;
			}
		}
		if( params->relation_params->disambiguate != 0 ) {
			switch( params->relation_params->disambiguate )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "disambiguate", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "disambiguate", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->relation_params->linked_data != 0 ) {
			switch( params->relation_params->linked_data )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "linkedData", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->relation_params->coreference != 0 ) {
			switch( params->relation_params->coreference )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "coreference", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "coreference", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->relation_params->show_source_text != 0 ) {
			switch( params->relation_params->show_source_text )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "showSourceText", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->relation_params->source_text != 0 ) {
			switch( params->relation_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CLEANED:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;				
			}
		}
		if( params->relation_params->cquery != 0 ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->relation_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->relation_params->xpath != 0 ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->relation_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->relation_params->base_url != 0 ) {
			if (-1 == alchemy_add_param(post_data, "baseUrl", params->relation_params->base_url))
				return ALCHEMY_INTERNAL_ERROR;
		}
                switch( params->relation_params->sentiment )
                {
                        case ALCHEMY_BOOL_FALSE:
                                if (-1 == alchemy_add_param(post_data, "sentiment", "0"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        case ALCHEMY_BOOL_TRUE:
                                if (-1 == alchemy_add_param(post_data, "sentiment", "1"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        default:
                                break;
                }
                switch( params->relation_params->entities )
                {
                        case ALCHEMY_BOOL_FALSE:
                                if (-1 == alchemy_add_param(post_data, "entities", "0"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        case ALCHEMY_BOOL_TRUE:
                                if (-1 == alchemy_add_param(post_data, "entities", "1"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        default:
                                break;
                }
                switch( params->relation_params->required_entities )
                {
                        case ALCHEMY_BOOL_FALSE:
                                if (-1 == alchemy_add_param(post_data, "requireEntities", "0"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        case ALCHEMY_BOOL_TRUE:
                                if (-1 == alchemy_add_param(post_data, "requireEntities", "1"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        default:
                                break;
                }
                switch( params->relation_params->sentiment_exclude_entities )
                {
                        case ALCHEMY_BOOL_FALSE:
                                if (-1 == alchemy_add_param(post_data, "sentimentExcludeEntities", "0"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        case ALCHEMY_BOOL_TRUE:
                                if (-1 == alchemy_add_param(post_data, "sentimentExcludeEntities", "1"))
                                        return ALCHEMY_INTERNAL_ERROR;
                                break;
                        default:
                                break;
                }
	}
	else if( params->category_params != 0 ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->category_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->category_params->source_text != 0 ) {
			switch( params->category_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->category_params->cquery != 0 ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->category_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->category_params->xpath != 0 ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->category_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->category_params->base_url != 0 ) {
			if (-1 == alchemy_add_param(post_data, "baseUrl", params->category_params->base_url))
				return ALCHEMY_INTERNAL_ERROR;
		}	
	
	}
	else if( params->language_params != 0 ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->language_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->language_params->source_text != 0 ) {
			switch( params->language_params->source_text )
			{
				case ALCHEMY_CLEANED_OR_RAW:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cleaned_or_raw"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_CQUERY:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "cquery"))
						return ALCHEMY_INTERNAL_ERROR;	
					break;
				case ALCHEMY_XPATH:	
					if (-1 == alchemy_add_param(post_data, "sourceText", "xpath"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->language_params->cquery != 0 ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->language_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if( params->language_params->xpath != 0 ) {
			if (-1 == alchemy_add_param(post_data, "xpath", params->language_params->xpath))
				return ALCHEMY_INTERNAL_ERROR;
		}
	}
	else if( params->text_params != 0 ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->text_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->text_params->use_metadata != 0 ) {
			switch( params->text_params->use_metadata )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "useMetadata", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "useMetadata", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}
		if( params->text_params->extract_links != 0 ) {
			switch( params->text_params->extract_links )
			{
				case ALCHEMY_BOOL_FALSE:	
					if (-1 == alchemy_add_param(post_data, "extractLinks", "0"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
				case ALCHEMY_BOOL_TRUE:	
					if (-1 == alchemy_add_param(post_data, "extractLinks", "1"))
						return ALCHEMY_INTERNAL_ERROR;
					break;
			}
		}		
	}
	else if( params->constraint_query_params != 0 ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, params->constraint_query_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->constraint_query_params->cquery != 0 ) {
			if (-1 == alchemy_add_param(post_data, "cquery", params->constraint_query_params->cquery))
				return ALCHEMY_INTERNAL_ERROR;
		}
	}
	else if( params->targeted_sentiment_params != 0) {
		if(-1 == alchemy_base_param_struct_handler(post_data, params->targeted_sentiment_params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
		if( params->targeted_sentiment_params->sentiment_phrase != 0) {
			if(-1 == alchemy_add_param(post_data, "target", params->targeted_sentiment_params->sentiment_phrase))
				return ALCHEMY_INTERNAL_ERROR;
		}
		if(params->targeted_sentiment_params->show_source_text != 0) {
			switch( params->targeted_sentiment_params->show_source_text )
			{
				case ALCHEMY_BOOL_FALSE:
                                        if (-1 == alchemy_add_param(post_data, "showSourceText", "0"))
                                                return ALCHEMY_INTERNAL_ERROR;
                                        break;
                                case ALCHEMY_BOOL_TRUE:
                                        if (-1 == alchemy_add_param(post_data, "showSourceText", "1"))
                                                return ALCHEMY_INTERNAL_ERROR;
                                        break;
                        }
		}
	}
	else if( params != 0 ) {
		if (-1 == alchemy_base_param_struct_handler(post_data, *params->base_params))
			return ALCHEMY_INTERNAL_ERROR;
	}
	
	return ALCHEMY_OK;
}

Alchemy_code alchemy_base_param_reset(Alchemy_base_params* param_struct)
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
			
	param_struct->url = NULL;
	param_struct->html = NULL;
	param_struct->text = NULL;
    }
    return ALCHEMY_OK;
}

Alchemy_code alchemy_super_param_reset(Alchemy_params* params) {
	if(NULL != params) {
		if(NULL != params->concept_params)
			return alchemy_base_param_reset(&params->concept_params->base_params);
		if(NULL != params->keyword_params)
			return alchemy_base_param_reset(&params->keyword_params->base_params);
		if(NULL != params->entity_params) 
			return alchemy_base_param_reset(&params->entity_params->base_params);
		if(NULL != params->relation_params)
			return alchemy_base_param_reset(&params->relation_params->base_params);
		if(NULL != params->category_params)
			return alchemy_base_param_reset(&params->category_params->base_params);
		if(NULL != params->language_params)
			return alchemy_base_param_reset(&params->language_params->base_params);
		if(NULL != params->text_params)
			return alchemy_base_param_reset(&params->text_params->base_params);
		if(NULL != params->constraint_query_params)
			return alchemy_base_param_reset(&params->constraint_query_params->base_params);
		if(NULL != params->targeted_sentiment_params)
			return alchemy_base_param_reset(&params->targeted_sentiment_params->base_params);
		if(NULL != params->base_params)
			return alchemy_base_param_reset(params->base_params);
	}
	return ALCHEMY_OK;
}


static Alchemy_code alchemy_prepare_request_parameters(
    alchemy_post* post_data, const alchemyAPI_obj* api_obj,
    const char* method, const char* prefix, Alchemy_params* params)
{	
    post_data->curl_hndlr = curl_easy_init();
    if (NULL == post_data->curl_hndlr) {
        return ALCHEMY_REQUEST_INIT_FAILED;
    }
    post_data->data = (char*)malloc((api_obj->key_len + 1u + strlen("apikey=&outputMode=xml")) * sizeof(char));
    strcpy(post_data->data, "apikey=");
    strncat(post_data->data, api_obj->key, api_obj->key_len);
    strcat(post_data->data, "&outputMode=xml");

    if (-1 == alchemy_param_struct_handler(post_data, params))
	  return ALCHEMY_INTERNAL_ERROR;

    alchemy_build_uri(post_data, api_obj, prefix, method);

    return ALCHEMY_OK;
}





static size_t alchemy_write_data(void* buffer, size_t size, size_t nmemb,
                               void* post_data_)
{
    if (NULL != buffer && NULL != post_data_ && 0u != size && 0u != nmemb)
    {
        const size_t len = size * nmemb;
        alchemy_post* post_data = (alchemy_post*)post_data_;

        char* buf = realloc(post_data->ret_data, post_data->ret_size + len);
        if (NULL == buf)
            nmemb = 0u;
        else
        {
            post_data->ret_data = buf;
            memcpy(post_data->ret_data + post_data->ret_size, buffer, len);
            post_data->ret_size += len;
        }
    }

    return nmemb;
}

static Alchemy_code alchemy_perform_request(alchemy_post* post_data)
{
    if (CURLE_OK != curl_easy_setopt(post_data->curl_hndlr, CURLOPT_POSTFIELDS,
                                     post_data->data)                         ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr,
                                     CURLOPT_POSTFIELDSIZE,
                                     strlen(post_data->data))                 ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr, CURLOPT_URL,
                                     post_data->uri)                          ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr,
                                     CURLOPT_WRITEFUNCTION, alchemy_write_data) ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr, CURLOPT_WRITEDATA,
                                     post_data)                               ||
        CURLE_OK != curl_easy_perform(post_data->curl_hndlr))
            return ALCHEMY_REQUEST_PERFORM_FAILED;
    return ALCHEMY_OK;
}





static Alchemy_code alchemy_perform_requestGET(alchemy_post* post_data){
	size_t len = strlen(post_data->uri);
	len += 2u; // '?', '\0'
	len += strlen(post_data->data);
        post_data->uri = realloc((void*)post_data->uri, len);
	strcat(post_data->uri,"?");
    	strcat(post_data->uri, post_data->data);
	if (CURLE_OK != curl_easy_setopt(post_data->curl_hndlr, CURLOPT_URL,
                                     post_data->uri)                          ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr,
                                     CURLOPT_WRITEFUNCTION, alchemy_write_data) ||
        CURLE_OK != curl_easy_setopt(post_data->curl_hndlr, CURLOPT_WRITEDATA,
                                     post_data)                               ||
        CURLE_OK != curl_easy_perform(post_data->curl_hndlr))
            return ALCHEMY_REQUEST_PERFORM_FAILED;
    return ALCHEMY_OK;
}

static Alchemy_code alchemy_check_response(const char* data, size_t size)
{
    Alchemy_code ret;
    xmlDocPtr doc = NULL;
    xmlNodePtr status_node = NULL;
    xmlXPathObjectPtr status = NULL;
    xmlXPathContextPtr context = NULL;

    if (NULL == data || !size)
        return ALCHEMY_ILLEGAL_ALCHEMY_RESPONSE;

    ret = ALCHEMY_ILLEGAL_ALCHEMY_RESPONSE;
    doc = xmlParseMemory(data, (int)size);
    if (NULL != doc)
    {
        context = xmlXPathNewContext(doc);
        if (NULL != context)
        {
            status = xmlXPathEvalExpression("/results/status/text()", context);
            if (NULL != status && !xmlXPathNodeSetIsEmpty(status->nodesetval))
            {
                status_node = xmlXPathNodeSetItem(status->nodesetval, 0);
                if (NULL != status_node && XML_TEXT_NODE == status_node->type &&
                    !alchemy_is_empty(status_node->content) &&
                    !strcmp("OK", (const char*)status_node->content))
                    ret = ALCHEMY_OK;
            }

            if (ALCHEMY_OK != ret)
            {
                status = xmlXPathEvalExpression("/results/statusInfo/text()", context);
                if (NULL != status && !xmlXPathNodeSetIsEmpty(status->nodesetval))
                {
                    status_node = xmlXPathNodeSetItem(status->nodesetval, 0);
                    if (NULL != status_node && XML_TEXT_NODE == status_node->type &&
                        !alchemy_is_empty(status_node->content))
                    {
                        if (!strcmp("invalid-api-key", (const char*)status_node->content))
                        {
                            ret = ALCHEMY_INVALID_API_KEY;
                        }
                        else if (!strcmp("daily-transaction-limit-exceeded", (const char*)status_node->content))
                        {
                            ret = ALCHEMY_TRANSACTION_LIMIT_EXCEEDED;
                        }
                        else if (!strcmp("content-exceeds-size-limit", (const char*)status_node->content))
                        {
                            ret = ALCHEMY_CONTENT_EXCEEDS_SIZE_LIMIT;
                        }
                        else if (strlen(status_node->content) >= strlen("cannot-retrieve") &&
                                 !strncmp((const char*)status_node->content, "cannot-retrieve", strlen("cannot-retrieve")))
                        {
                            ret = ALCHEMY_CANNOT_RETRIEVE;
                        }
                    }
                }
            }
        }

        if (NULL != context)
            xmlXPathFreeContext(context);
        if (NULL != status)
            xmlXPathFreeObject(status);
        if (NULL != doc)
            xmlFreeDoc(doc);
    }

    return ret;
}

Alchemy_code alchemy_post_request(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, uint8_t paramsNum, ...)
{
    Alchemy_code ret;
    alchemy_post post_data = {0u};
    post_data.paramsNum = paramsNum;
    va_start(post_data.params, paramsNum);

    ret = alchemy_prepare_request(&post_data, api_obj, method, prefix);
    if (ALCHEMY_OK == ret)
    {
        ret = alchemy_perform_request(&post_data);
        if (ALCHEMY_OK == ret && NULL != post_data.ret_data && post_data.ret_size)
        {
            ret = alchemy_check_response(post_data.ret_data, post_data.ret_size);
            if (NULL != post_data.ret_data && post_data.ret_size > 0u)
                callback(post_data.ret_data, post_data.ret_size, data);
        }
    }

    alchemy_free(&post_data);
    return ret;
}

Alchemy_code alchemy_post_request_parameters(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, Alchemy_params* params, ...)
{
    Alchemy_code ret;
    alchemy_post post_data = {0u};

    ret = alchemy_prepare_request_parameters(&post_data, api_obj, method, prefix, params);
    if (ALCHEMY_OK == ret)
    {
        ret = alchemy_perform_request(&post_data);
        if (ALCHEMY_OK == ret && NULL != post_data.ret_data && post_data.ret_size)
        {
            ret = alchemy_check_response(post_data.ret_data, post_data.ret_size);
            if (NULL != post_data.ret_data && post_data.ret_size > 0u)
                callback(post_data.ret_data, post_data.ret_size, data);
        }
    }
	alchemy_super_param_reset(params);
    alchemy_free(&post_data);
    return ret;
}

Alchemy_code alchemy_get_request(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, uint8_t paramsNum, ...)
{
    Alchemy_code ret;
    alchemy_post post_data = {0u};
    post_data.paramsNum = paramsNum;
    va_start(post_data.params, paramsNum);

    ret = alchemy_prepare_request(&post_data, api_obj, method, prefix);
    if (ALCHEMY_OK == ret)
    {
        ret = alchemy_perform_requestGET(&post_data);
        if (ALCHEMY_OK == ret && NULL != post_data.ret_data && post_data.ret_size)
        {
            ret = alchemy_check_response(post_data.ret_data, post_data.ret_size);
            if (NULL != post_data.ret_data && post_data.ret_size > 0u)
                callback(post_data.ret_data, post_data.ret_size, data);
        }
    }

    alchemy_free(&post_data);
    return ret;
}

Alchemy_code alchemy_get_request_parameters(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, Alchemy_params* params, ...)
{
    Alchemy_code ret;
    alchemy_post post_data = {0u};

    ret = alchemy_prepare_request_parameters(&post_data, api_obj, method, prefix, params);
    if (ALCHEMY_OK == ret)
    {
        ret = alchemy_perform_requestGET(&post_data);
        if (ALCHEMY_OK == ret && NULL != post_data.ret_data && post_data.ret_size)
        {
            ret = alchemy_check_response(post_data.ret_data, post_data.ret_size);
            if (NULL != post_data.ret_data && post_data.ret_size > 0u)
                callback(post_data.ret_data, post_data.ret_size, data);
        }
    }

    alchemy_super_param_reset(params);
    alchemy_free(&post_data);
    return ret;
}

int alchemy_check_key(const alchemyAPI_obj* api_obj)
{
    return (NULL != api_obj && NULL != api_obj->key && api_obj->key_len >= 5u);
}

int alchemy_check_host(const alchemyAPI_obj* api_obj)
{
    return (NULL != api_obj && NULL != api_obj->host && api_obj->host_len >= 5u);
}

int alchemy_check_url(const char* url)
{
    return (!alchemy_is_empty(url) && strlen(url) >= 10);
}

int alchemy_check_html(const char* html)
{
    return (!alchemy_is_empty(html) && strlen(html) >= 5);
}

int alchemy_check_text(const char* text)
{
    return (!alchemy_is_empty(text) && strlen(text) >= 5);
}

int alchemy_check_constraint_query(const char* query)
{
    return (!alchemy_is_empty(query) && strlen(query) >= 2);
}
