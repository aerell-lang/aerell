/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef RANGES_H
#define RANGES_H

#include <stddef.h>

#include "aerell/support/range/range.h"

bool is_in_uint32_ranges(uint32_t cp, const uint32_range_t* ranges, size_t ranges_count);

#endif