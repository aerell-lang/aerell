// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "aerell/file.h"

bool file_load(file_t* self, const char* path)
{
    assert(self != NULL && path != NULL && "self or path is null");

    FILE* cfile = fopen(path, "rb");
    if(cfile == NULL) return false;

    fseek(cfile, 0, SEEK_END);
    uintptr_t len = ftell(cfile);
    rewind(cfile);

    char* buffer = (char*)malloc(sizeof(char) * (len + 1));
    assert(buffer != NULL && "malloc failed");
    if(buffer == NULL)
    {
        fclose(cfile);
        return false;
    }

    uintptr_t rlen = fread(buffer, 1, len, cfile);
    assert(rlen == len && "rlen != len");
    if(rlen != len)
    {
        free(buffer);
        fclose(cfile);
        return false;
    }

    buffer[len] = '\0';

    fclose(cfile);

    self->path = path;
    self->content.data = buffer;
    self->content.len = len;

    return true;
}

void file_unload(file_t* self)
{
    assert(self != NULL && "self is null");
    free((char*)self->content.data);
    self->content.len = 0;
}