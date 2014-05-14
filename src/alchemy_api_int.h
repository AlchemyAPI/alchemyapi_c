/*************************************************************************
 * Summary: AlchemyAPI
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ALCHEMY_API_INT_H__
#define __ALCHEMY_API_INT_H__

#include <stdint.h>
#include <curl/curl.h>

#include "alchemy_api.h"

#ifdef __cplusplus
extern "C" {
#endif

int alchemy_is_empty(const char* str);

Alchemy_code alchemy_post_request(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, uint8_t paramsNum, ...);

Alchemy_code alchemy_get_request(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, uint8_t paramsNum, ...);
	
Alchemy_code alchemy_post_request_parameters(
    const alchemyAPI_obj* api_obj, alchemy_ret_callback callback, void* data,
    const char* method, const char* prefix, Alchemy_params* params, ...);

int alchemy_check_key(const alchemyAPI_obj* api_obj);

int alchemy_check_host(const alchemyAPI_obj* api_obj);

int alchemy_check_url(const char* url);

int alchemy_check_html(const char* html);

int alchemy_check_text(const char* text);

int alchemy_check_constraint_query(const char* query);

#ifdef __cplusplus
}
#endif

#endif /* __ALCHEMY_API_INT_H__ */
