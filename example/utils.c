/*************************************************************************
 * Summary: AlchemyAPI Example
 * Copyright: (C) 2009-2010, Orchestr8, LLC - See COPYING file.
 *************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "alchemy_api.h"


void alchemy_stdout_callback(const char* buf, size_t len, void* data)
{
    if (NULL != buf && len > 0u)
        fwrite(buf, sizeof(char), len, stdout);
}

void alchemy_error_exit(const char* msg, alchemyAPI_obj* api_obj, char* data)
{
    alchemy_free_api_obj_util(api_obj);
    if (NULL != data)
        free(data);

    if (!alchemy_is_empty(msg))
        fprintf(stderr, "\n%s\n", msg);

    alchemy_global_cleanup();

    exit(1);
}

void alchemy_exit(alchemyAPI_obj* api_obj, char* data)
{
    alchemy_free_api_obj_util(api_obj);
    alchemy_global_cleanup();

    if (NULL != data)
        free(data);

    exit(0);
}

int alchemy_load_data(char** data, const char* path)
{
    int ret;
    long size;
    FILE* file;

    if (alchemy_is_empty(path))
        return 0;

    file = fopen(path, "rb");
    if (NULL == file)
        return 0;

    if (-1 == fseek(file, 0, SEEK_END) || -1 == (size = ftell(file)))
    {
        fclose(file);
        return 0;
    }
    rewind(file);

    ret = 0;
    *data = NULL;
    if (size > 0L)
    {
        *data = (char*)malloc(sizeof(char) * size + 1u);
        if (NULL != *data &&
            fread((void*)*data, sizeof(char), size, file) == size)
        {
            (*data)[size] = '\0';
            ret = 1;
        }
    }

    if (!ret)
    {
        free(*data);
        *data = NULL;
    }
    fclose (file);

    return ret;
}
