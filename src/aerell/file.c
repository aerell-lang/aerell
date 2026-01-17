// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifdef INCLUDE
#pragma once
#endif

#ifndef INCLUDE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#endif

typedef struct
{
    const char* path;
    const char* buffer;
    size_t len;
} File;

bool file_load(File* file, const char* path)
#ifdef INCLUDE
    ;
#else
{
    file->path = path;
    file->buffer = NULL;

    FILE* fptr = fopen(path, "rb");
    if(fptr == NULL) return false;

    fseek(fptr, 0, SEEK_END);
    size_t len = ftell(fptr);
    rewind(fptr);

    char* buffer = malloc(len + 1);
    if(buffer == NULL)
    {
        fclose(fptr);
        return false;
    }

    size_t rlen = fread(buffer, 1, len, fptr);
    if(rlen != len)
    {
        free(buffer);
        fclose(fptr);
        return false;
    }

    buffer[len] = '\0';

    file->buffer = buffer;
    file->len = len;

    fclose(fptr);
    return true;
}
#endif

void file_unload(File* file)
#ifdef INCLUDE
    ;
#else
{
    free((char*)file->buffer);
}
#endif