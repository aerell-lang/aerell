/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef UNICODE_RANGES_H
#define UNICODE_RANGES_H

#include <stddef.h>
#include <stdint.h>

#include "aerell/support/range/range.h"

// Ranges for xid_start
extern const uint32_range_t xid_start_ranges[];
extern const size_t xid_start_ranges_count;

// Ranges for xid_continue
extern const uint32_range_t xid_continue_ranges[];
extern const size_t xid_continue_ranges_count;

// Ranges for whitespace
extern const uint32_range_t whitespace_ranges[];
extern const size_t whitespace_ranges_count;

#endif