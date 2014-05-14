/*************************************************************************
 * Summary: AlchemyAPI
 * Copyright: (C) 2014, AlchemyAPI, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMY_API_H__
#define __ALCHEMY_API_H__

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    ALCHEMY_OK = 0,

    ALCHEMY_CURL_INIT_FAILED,
    ALCHEMY_REQUEST_INIT_FAILED,
    ALCHEMY_REQUEST_PERFORM_FAILED,

    ALCHEMY_ILLEGAL_KEY_SPECIFIED,
    ALCHEMY_ILLEGAL_URL_SPECIFIED,
    ALCHEMY_ILLEGAL_HTML_SPECIFIED,
    ALCHEMY_ILLEGAL_TEXT_SPECIFIED,
    ALCHEMY_ILLEGAL_HOST_SPECIFIED,
    ALCHEMY_CALLBACK_ISNT_SPECIFIED,
    ALCHEMY_UNABLE_TO_READ_KEY,
    ALCHEMY_INVALID_API_KEY,
    ALCHEMY_TRANSACTION_LIMIT_EXCEEDED,
    ALCHEMY_CONTENT_EXCEEDS_SIZE_LIMIT,
    ALCHEMY_CANNOT_RETRIEVE,
    ALCHEMY_INSUFFICIENT_PARAMS_SPECIFIED,

    ALCHEMY_ILLEGAL_ALCHEMY_RESPONSE,

    ALCHEMY_INTERNAL_ERROR,

    ALCHEMY_LAST
} Alchemy_code;

typedef enum {ALCHEMY_BOOL_NONE = 0, ALCHEMY_BOOL_FALSE, ALCHEMY_BOOL_TRUE} TBOOL;

typedef enum {
    ALCHEMY_KE_NONE = 0,
    ALCHEMY_NORMAL,
    ALCHEMY_STRICT
} Alchemy_keyword_extract_mode;

typedef enum {
    ALCHEMY_ST_NONE = 0,
    ALCHEMY_CLEANED_OR_RAW,
    ALCHEMY_CLEANED,
    ALCHEMY_RAW,
    ALCHEMY_CQUERY,
    ALCHEMY_XPATH
} Alchemy_source_text;

typedef enum {
    ALCHEMY_ALWAYS_INFER,
    ALCHEMY_TRUST_META
} Alchemy_image_extract_mode;

typedef enum {
    ALCHEMY_PARAMETERS_URL,
    ALCHEMY_PARAMETERS_HTML,
    ALCHEMY_PARAMETERS_TEXT,
    ALCHEMY_PARAMETERS_MAX_RETRIEVE,
    ALCHEMY_PARAMETERS_DISAMBIGUATE,
    ALCHEMY_PARAMETERS_LINKED_DATA,
    ALCHEMY_PARAMETERS_COREFERENCE,
    ALCHEMY_PARAMETERS_QUOTATIONS,
    ALCHEMY_PARAMETERS_SOURCE_TEXT,
    ALCHEMY_PARAMETERS_SHOW_SOURCE_TEXT,
    ALCHEMY_PARAMETERS_CQUERY,
    ALCHEMY_PARAMETERS_XPATH,
    ALCHEMY_PARAMETERS_BASE_URL,
    ALCHEMY_PARAMETERS_KEYWORD_EXTRACT_MODE,
    ALCHEMY_PARAMETERS_USE_METADATA,
    ALCHEMY_PARAMETERS_EXTRACT_LINKS,
    ALCHEMY_PARAMETERS_SENTIMENT,
    ALCHEMY_PARAMETERS_ENTITIES,
    ALCHEMY_PARAMETERS_REQUIRED_ENTITIES,
    ALCHEMY_PARAMETERS_EXCLUDE_SENTI_ENTITIES,
    ALCHEMY_PARAMETERS_SENTIMENT_PHRASE,
    ALCHEMY_PARAMETERS_IMAGE_EXTRACT_MODE,
    ALCHEMY_PARAMETERS_EXTRACT
    //ALCHEMY_PARAMETERS_CUSTOM_PARAMETERS
} Alchemy_parameter_names;


typedef struct
{
    const char* key;
    size_t key_len;
    const char* host;
    size_t host_len;
} alchemyAPI_obj;

typedef struct
{
    char* url;
    char* html;
    char* text;

    char* custom_params;

} Alchemy_base_params;

typedef struct
{
    Alchemy_base_params base_params;
    int max_retrieve;
    TBOOL show_source_text;
    Alchemy_source_text source_text;
    TBOOL linked_data;
    char* cquery;
    char* xpath;

} Alchemy_concept_params;

typedef struct
{
    Alchemy_base_params base_params;
    int max_retrieve;
    Alchemy_keyword_extract_mode keyword_extract_mode;
    TBOOL show_source_text;
    TBOOL sentiment;
    Alchemy_source_text source_text;
    char* cquery;
    char* xpath;
    char* base_url;

} Alchemy_keyword_params;

typedef struct
{
    Alchemy_base_params base_params;
    int max_retrieve;
    TBOOL disambiguate;
    TBOOL linked_data;
    TBOOL coreference;
    TBOOL quotations;
    Alchemy_source_text source_text;
    TBOOL show_source_text;
    char* cquery;
    char* xpath;
    char* base_url;
    TBOOL sentiment;

} Alchemy_entity_params;

typedef struct
{
    Alchemy_base_params base_params;
    int max_retrieve;
    TBOOL show_source_text;
    Alchemy_source_text source_text;
    TBOOL sentiment;
    TBOOL entities;
    TBOOL required_entities;
    TBOOL sentiment_exclude_entities;
    TBOOL disambiguate;
    TBOOL linked_data;
    TBOOL coreference;
    char* cquery;
    char* xpath;
    char* base_url;

} Alchemy_relation_params;

typedef struct
{
    Alchemy_base_params base_params;
    Alchemy_source_text source_text;
    char* cquery;
    char* xpath;
    char* base_url;

} Alchemy_category_params;

typedef struct
{
    Alchemy_base_params base_params;
    Alchemy_source_text source_text;
    char* cquery;
    char* xpath;

} Alchemy_language_params;

typedef struct
{
    Alchemy_base_params base_params;
    TBOOL use_metadata;
    TBOOL extract_links;

} Alchemy_text_params;

typedef struct
{
    Alchemy_base_params base_params;
    char* cquery;

} Alchemy_constraint_query_params;

typedef struct
{
    Alchemy_base_params base_params;
    char* sentiment_phrase;
    TBOOL show_source_text;
} Alchemy_targeted_sentiment_params;

typedef struct
{
    Alchemy_base_params base_params;
    Alchemy_source_text source_text;
    char* cquery;
    char* xpath;
    char* base_url;
} Alchemy_taxonomy_params;

typedef struct
{
    Alchemy_base_params base_params;
    Alchemy_image_extract_mode image_extract_mode;
} Alchemy_image_params;

typedef struct
{
    Alchemy_base_params base_params;
    int max_retrieve;
    TBOOL disambiguate;
    TBOOL linked_data;
    TBOOL coreference;
    TBOOL quotations;
    Alchemy_source_text source_text;
    TBOOL show_source_text;
    char* cquery;
    char* xpath;
    char* base_url;
    Alchemy_keyword_extract_mode keyword_extract_mode;
    TBOOL use_metadata;
    TBOOL extract_links;
    char* sentiment_phrase;
    TBOOL sentiment;
    TBOOL entities;
    TBOOL required_entities;
    TBOOL sentiment_exclude_entities;
    Alchemy_image_extract_mode image_extract_mode;
    char* extract;
} Alchemy_combined_data_params;

typedef struct
{
    Alchemy_concept_params* concept_params;
    Alchemy_keyword_params* keyword_params;
    Alchemy_entity_params* entity_params;
    Alchemy_relation_params* relation_params;
    Alchemy_category_params* category_params;
    Alchemy_language_params* language_params;
    Alchemy_text_params* text_params;
    Alchemy_constraint_query_params* constraint_query_params;
    Alchemy_targeted_sentiment_params* targeted_sentiment_params;
    Alchemy_taxonomy_params* taxonomy_params;
    Alchemy_image_params* image_params;
    Alchemy_combined_data_params* combined_data_params;
    Alchemy_base_params* base_params;
} Alchemy_params;

Alchemy_code alchemy_set_key_util(alchemyAPI_obj* api_obj, const char* keyText);

Alchemy_code alchemy_load_key_util(alchemyAPI_obj* api_obj, const char* path);

Alchemy_code alchemy_set_api_host(alchemyAPI_obj* api_obj, const char* host);

void alchemy_free_api_obj_util(alchemyAPI_obj* api_obj);

void alchemy_free_key_util(alchemyAPI_obj* api_obj);

void alchemy_free_host_util(alchemyAPI_obj* api_obj);

typedef void (*alchemy_ret_callback)(const char* buf, size_t len, void* data);

Alchemy_code alchemy_global_init(void);

void alchemy_global_cleanup(void);

Alchemy_code alchemy_url_get_author(const alchemyAPI_obj* api_obj, const char* url,
				  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_author_params(const alchemyAPI_obj* api_obj, const char* html, const char* url,
				  alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_url_get_author_params(const alchemyAPI_obj* api_obj, const char* url,
				  alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_author(const alchemyAPI_obj* api_obj, const char* html, const char* url,
				  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_category(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_category(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_category(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_category_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params);

Alchemy_code alchemy_html_get_category_params(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params);

Alchemy_code alchemy_text_get_category_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_category_params* category_params);

Alchemy_code alchemy_url_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_ranked_named_entities(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params);

Alchemy_code alchemy_text_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params);

Alchemy_code alchemy_html_get_ranked_named_entities_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_entity_params* entity_params);

Alchemy_code alchemy_url_get_relations(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_relations(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_relations(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params);

Alchemy_code alchemy_text_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* text,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params);

Alchemy_code alchemy_html_get_relations_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_relation_params* relation_params);

Alchemy_code alchemy_url_get_feed_links(const alchemyAPI_obj* api_obj, const char* url,
                                    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_feed_links(const alchemyAPI_obj* api_obj,
                                     const char* html, const char* url,
                                     alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_feed_links_params(const alchemyAPI_obj* api_obj, const char* url,
                                    alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_feed_links_params(const alchemyAPI_obj* api_obj,
                                     const char* html, const char* url,
                                     alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_url_get_ranked_concepts(const alchemyAPI_obj* api_obj, const char* url,
                                         alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_ranked_concepts(const alchemyAPI_obj* api_obj, const char* text,
                                          alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_ranked_concepts(const alchemyAPI_obj* api_obj,
                                          const char* html, const char* url,
                                          alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_ranked_concepts_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params);

Alchemy_code alchemy_text_get_ranked_concepts_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params);

Alchemy_code alchemy_html_get_ranked_concepts_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_concept_params* concept_params);

Alchemy_code alchemy_url_get_ranked_keywords(const alchemyAPI_obj* api_obj, const char* url,
                                         alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_ranked_keywords(const alchemyAPI_obj* api_obj, const char* text,
                                          alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_ranked_keywords(const alchemyAPI_obj* api_obj,
                                          const char* html, const char* url,
                                          alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_ranked_keywords_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params);
  
Alchemy_code alchemy_text_get_ranked_keywords_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params);

Alchemy_code alchemy_html_get_ranked_keywords_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url,
    alchemy_ret_callback callback, void* data, Alchemy_keyword_params* keyword_params);

Alchemy_code alchemy_url_get_language(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_language(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_language(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_language_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params);

Alchemy_code alchemy_text_get_language_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params);

Alchemy_code alchemy_html_get_language_params(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data, Alchemy_language_params* language_params);

Alchemy_code alchemy_url_get_microformats(const alchemyAPI_obj* api_obj, const char* url,
                                      alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_microformats(const alchemyAPI_obj* api_obj,
                                       const char* html, const char* url,
                                       alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_microformats_params(const alchemyAPI_obj* api_obj, const char* url,
                                      alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_microformats_params(const alchemyAPI_obj* api_obj,
                                       const char* html, const char* url,
                                       alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);


Alchemy_code alchemy_url_get_text(const alchemyAPI_obj* api_obj, const char* url,
                              alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_text(const alchemyAPI_obj* api_obj,
                               const char* html, const char* url,
                               alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_text_params(const alchemyAPI_obj* api_obj, const char* url,
                              alchemy_ret_callback callback, void* data, Alchemy_text_params* text_params);

Alchemy_code alchemy_html_get_text_params(const alchemyAPI_obj* api_obj,
                               const char* html, const char* url,
                               alchemy_ret_callback callback, void* data, Alchemy_text_params* text_params);

Alchemy_code alchemy_url_get_raw_text(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_raw_text(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_raw_text_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_raw_text_params(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_url_get_title(const alchemyAPI_obj* api_obj, const char* url,
                               alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_title(const alchemyAPI_obj* api_obj,
                                const char* html, const char* url,
                                alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_title_params(const alchemyAPI_obj* api_obj, const char* url,
                               alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_title_params(const alchemyAPI_obj* api_obj,
                                const char* html, const char* url,
                                alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_url_get_textsentiment(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_textsentiment(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_textsentiment(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_textsentiment_params(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_html_get_textsentiment_params(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_text_get_textsentiment_params(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data, Alchemy_base_params* base_params);

Alchemy_code alchemy_text_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* text, const char* target,
				   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* url, const char* target,
				   alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params);

Alchemy_code alchemy_html_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* target,
				   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* text, const char* target,
				   alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params);

Alchemy_code alchemy_url_get_targeted_sentiment(const alchemyAPI_obj* api_obj, const char* text, const char* target,
				   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_targeted_sentiment_params(const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* target,
				   alchemy_ret_callback callback, void* data, Alchemy_targeted_sentiment_params* targeted_sentiment_params);

Alchemy_code alchemy_url_get_constraint_query(
    const alchemyAPI_obj* api_obj, const char* url, const char* query,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_constraint_query(
    const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* query,
    alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_constraint_query_params(
    const alchemyAPI_obj* api_obj, const char* url, const char* query,
    alchemy_ret_callback callback, void* data, Alchemy_constraint_query_params* constraint_query_params);

Alchemy_code alchemy_html_get_constraint_query_params(
    const alchemyAPI_obj* api_obj, const char* html, const char* url, const char* query,
    alchemy_ret_callback callback, void* data, Alchemy_constraint_query_params* constraint_query_params);

Alchemy_code alchemy_url_get_ranked_taxonomy(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_html_get_ranked_taxonomy(const alchemyAPI_obj* api_obj,
                                   const char* html, const char* url,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_text_get_ranked_taxonomy(const alchemyAPI_obj* api_obj, const char* text,
                                   alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_taxonomy_params(const alchemyAPI_obj* api_obj, const char* url,
	alchemy_ret_callback callback, void* data, Alchemy_taxonomy_params* taxonomy_params);

Alchemy_code alchemy_html_get_taxonomy_params(const alchemyAPI_obj* api_obj,
	const char* html, const char* url, alchemy_ret_callback callback, void* data,
	Alchemy_taxonomy_params* taxonomy_params);

Alchemy_code alchemy_text_get_taxonomy_params(const alchemyAPI_obj* api_obj, const char* text,
	alchemy_ret_callback callback, void* data, Alchemy_taxonomy_params* taxonomy_params);

Alchemy_code alchemy_url_get_image(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_image_params(const alchemyAPI_obj* api_obj, const char* url,
	alchemy_ret_callback callback, void* data, Alchemy_image_params* image_params);

Alchemy_code alchemy_url_get_combined_data(const alchemyAPI_obj* api_obj, const char* url,
                                  alchemy_ret_callback callback, void* data);

Alchemy_code alchemy_url_get_combined_data_params(const alchemyAPI_obj* api_obj, const char* url,
	alchemy_ret_callback callback, void* data, Alchemy_combined_data_params* combined_data_params);



Alchemy_concept_params alchemy_concept_param_init();

Alchemy_code alchemy_concept_param_set_string(Alchemy_concept_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_concept_param_set_int(Alchemy_concept_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_concept_param_set_custom_params(Alchemy_concept_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_concept_param_free(Alchemy_concept_params* param_struct);


Alchemy_keyword_params alchemy_keyword_param_init();

Alchemy_code alchemy_keyword_param_set_string(Alchemy_keyword_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_keyword_param_set_int(Alchemy_keyword_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_keyword_param_set_custom_params(Alchemy_keyword_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_keyword_param_free(Alchemy_keyword_params* param_struct);


Alchemy_category_params alchemy_category_param_init();

Alchemy_code alchemy_category_param_set_string(Alchemy_category_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_category_param_set_int(Alchemy_category_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_category_param_set_custom_params(Alchemy_category_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_category_param_free(Alchemy_category_params* param_struct);


Alchemy_entity_params alchemy_entity_param_init();

Alchemy_code alchemy_entity_param_set_string(Alchemy_entity_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_entity_param_set_int(Alchemy_entity_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_entity_param_set_custom_params(Alchemy_entity_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_entity_param_free(Alchemy_entity_params* param_struct);


Alchemy_relation_params alchemy_relation_param_init();

Alchemy_code alchemy_relation_param_set_string(Alchemy_relation_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_relation_param_set_int(Alchemy_relation_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_relation_param_free(Alchemy_relation_params* param_struct);


Alchemy_base_params alchemy_base_param_init();

Alchemy_code alchemy_base_param_set_string(Alchemy_base_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_base_param_set_int(Alchemy_base_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_base_param_set_custom_params(Alchemy_base_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_base_param_free(Alchemy_base_params* param_struct);


Alchemy_language_params alchemy_language_param_init();

Alchemy_code alchemy_language_param_set_string(Alchemy_language_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_language_param_set_int(Alchemy_language_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_language_param_set_custom_params(Alchemy_language_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_language_param_free(Alchemy_language_params* param_struct);


Alchemy_text_params alchemy_text_param_init();

Alchemy_code alchemy_text_param_set_string(Alchemy_text_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_text_param_set_int(Alchemy_text_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_text_param_set_custom_params(Alchemy_text_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_text_param_free(Alchemy_text_params* param_struct);


Alchemy_constraint_query_params alchemy_constraint_query_param_init();

Alchemy_code alchemy_constraint_query_param_set_string(Alchemy_constraint_query_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_constraint_query_param_set_int(Alchemy_constraint_query_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_constraint_query_param_set_custom_params(Alchemy_constraint_query_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_constraint_query_param_free(Alchemy_constraint_query_params* param_struct);


Alchemy_targeted_sentiment_params alchemy_targeted_sentiment_param_init();

Alchemy_code alchemy_targeted_sentiment_param_set_string(Alchemy_targeted_sentiment_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_targeted_sentiment_param_set_int(Alchemy_targeted_sentiment_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_constraint_targeted_sentiment_param_set_custom_params(Alchemy_targeted_sentiment_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_targeted_sentiment_param_free(Alchemy_targeted_sentiment_params* param_struct);


Alchemy_taxonomy_params alchemy_taxonomy_param_init();

Alchemy_code alchemy_taxonomy_param_set_string(Alchemy_taxonomy_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_taxonomy_param_set_int(Alchemy_taxonomy_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_taxonomy_param_set_custom_params(Alchemy_taxonomy_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_taxonomy_param_free(Alchemy_taxonomy_params* param_struct);


Alchemy_image_params alchemy_image_param_init();

Alchemy_code alchemy_image_param_set_string(Alchemy_image_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_image_param_set_int(Alchemy_image_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_image_param_set_custom_params(Alchemy_image_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_image_param_free(Alchemy_image_params* param_struct);


Alchemy_combined_data_params alchemy_combined_data_param_init();

Alchemy_code alchemy_combined_data_param_set_string(Alchemy_combined_data_params* param_struct, Alchemy_parameter_names param, const char* value);

Alchemy_code alchemy_combined_data_param_set_int(Alchemy_combined_data_params* param_struct, Alchemy_parameter_names param, int value);

Alchemy_code alchemy_combined_data_param_set_custom_params(Alchemy_combined_data_params* param_struct, int8_t params_num,...);

Alchemy_code alchemy_combined_data_param_free(Alchemy_combined_data_params* param_struct);


Alchemy_code alchemy_set_custom_params(Alchemy_base_params* base_params, int8_t params_num, va_list temp_list);

#ifdef __cplusplus
}
#endif

#endif /* __ALCHEMY_API_H__ */
