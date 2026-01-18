// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifdef INCLUDE_FILE
#pragma once
#endif

#ifndef INCLUDE_FILE
#include <assert.h>
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
#ifdef INCLUDE_FILE
    ;
#else
{
    assert(file != NULL && "file is null");
    if(file == NULL) return false;

    file->path = path;
    file->buffer = NULL;

    FILE* fptr = fopen(path, "rb");
    if(fptr == NULL) return false;

    fseek(fptr, 0, SEEK_END);
    size_t len = ftell(fptr);
    rewind(fptr);

    char* buffer = malloc(len + 1);
    assert(buffer != NULL && "buffer is null, malloc failed");
    if(buffer == NULL)
    {
        fclose(fptr);
        return false;
    }

    size_t rlen = fread(buffer, 1, len, fptr);
    assert(rlen == len && "rlen don't same with len, something wrong with fread");
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
#ifdef INCLUDE_FILE
    ;
#else
{
    assert(file != NULL && "file is null");
    if(file == NULL) return;
    free((char*)file->buffer);
}
#endif
