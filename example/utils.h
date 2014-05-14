/*************************************************************************
 * Summary: AlchemyAPI Example
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#ifndef __ORCHESTR8_API_EXAMPLES_UTILS_H__
#define __ORCHESTR8_API_EXAMPLES_UTILS_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    const char* data;
    size_t size;
} alchemy_data;

void alchemy_stdout_callback(const char* buf, size_t len, void* data);

void alchemy_error_exit(const char* msg, alchemyAPI_obj* api_obj, char* data);

void alchemy_exit(alchemyAPI_obj* api_obj, char* data);

int alchemy_load_data(char** data, const char* path);

#ifdef __cplusplus
}
#endif

#endif /* __ORCHESTR8_API_EXAMPLES_UTILS_H__ */
