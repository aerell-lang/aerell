#ifndef RANGE_TABLE_H
#define RANGE_TABLE_H

#include <stddef.h>

#include "support/range/range.h"

bool in_uint32_range_table(const uint32_range_t* table, size_t count, uint32_t cp);

#endif