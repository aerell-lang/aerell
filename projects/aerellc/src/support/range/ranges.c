/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include "support/range/ranges.h"

bool is_in_uint32_ranges(uint32_t cp, const uint32_range_t* ranges, size_t ranges_count)
{
    size_t left = 0;
    size_t right = ranges_count;

    while(left < right)
    {
        size_t mid = left + (right - left) / 2;

        if(cp < ranges[mid].start)
        {
            right = mid;
        }
        else if(cp > ranges[mid].end)
        {
            left = mid + 1;
        }
        else
        {
            return true; // cp is in the range
        }
    }
    return false;
}