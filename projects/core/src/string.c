#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/string.h"

char* substring(const char* start, const char* end)
{
    if(start >= end) return NULL;
    size_t len = end - start;
    char* result = malloc(len + 1);
    if(!result) return NULL;
    memcpy(result, start, len);
    result[len] = '\0';
    return result;
}

char* strdupWithChar(char c)
{
    char* result = malloc(2 * sizeof(char));
    if(!result) return NULL;
    result[0] = c;
    result[1] = '\0';
    return result;
}