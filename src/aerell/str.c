// Copyright 2026 Fern Aerell
// SPDX-License-Identifier: Apache-2.0

#include <assert.h>
#include <string.h>

#include "aerell/str.h"

str_t str_from_c_str(const char* data)
{
    assert(data != NULL && "data is null");
    return (str_t){.data = data, .len = strlen(data)};
}

str_t str_sub(const str_t* self, uintptr_t pos, uintptr_t count)
{
    assert(self != NULL && "self is null");
    return (str_t){.data = self->data + pos, .len = count};
}

str_t str_sub_range(const str_t* self, uintptr_t start, uintptr_t end)
{
    assert(self != NULL && "self is null");
    return str_sub(self, start, end - start);
}