// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/file/file.h"

#include <stdio.h>
#include <stdlib.h>

char* file_read(const char* path)
{
    if(path == NULL) return NULL;

    // Open file
    FILE* file = fopen(path, "rb");
    if(file == NULL) return NULL;

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file
    char* buffer = malloc(len + 1); // +1 for null terminator
    if(buffer == NULL)
    {
        fclose(file);
        return NULL;
    }

    size_t read_len = fread(buffer, 1, len, file);
    if(len != read_len)
    {
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[len] = '\0';

    // Close file
    fclose(file);

    return buffer;
}

file_t* file_load(const char* path)
{
    if(path == NULL) return NULL;

    // Read file
    char* content = file_read(path);
    if(content == NULL) return NULL;

    // Create file
    file_t* file = malloc(sizeof(file_t));
    if(file == NULL) return NULL;

    file->path = path;
    file->content = content;

    return file;
}

void file_free(file_t* file)
{
    if(file->content != NULL) free((void*)file->content);
}