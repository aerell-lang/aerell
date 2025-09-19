#ifndef UNICODE_RANGE_TABLES_H
#define UNICODE_RANGE_TABLES_H

#include <stddef.h>
#include <stdint.h>

#include "support/range/range.h"

// Table for xid_start
extern const uint32_range_t xid_start_range_table[];

extern const size_t xid_start_range_table_count;

// Table for xid_continue
extern const uint32_range_t xid_continue_range_table[];

extern const size_t xid_continue_range_table_count;

// Table for whitespace
extern const uint32_range_t whitespace_range_table[];

extern const size_t whitespace_range_table_count;

#endif