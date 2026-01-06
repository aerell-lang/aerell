// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifndef AERELL_FILE_FILE_H
#define AERELL_FILE_FILE_H

typedef struct
{
    const char* path;
    const char* content;
} file_t;

file_t* file_load(const char* path);

void file_free(file_t* file);

#endif