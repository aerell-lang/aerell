#ifndef RANGES_H
#define RANGES_H

#include <stddef.h>

#include "support/range/range.h"

bool is_in_uint32_ranges(uint32_t cp, const uint32_range_t* ranges, size_t ranges_count);

#endif