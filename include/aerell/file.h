// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/str.h"

typedef struct
{
    const char* path;
    str_t content;
} file_t;

bool file_load(file_t* self, const char* path);

void file_unload(file_t* self);