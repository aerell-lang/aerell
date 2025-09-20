/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include "aerell/support/unicode/unicode.h"
#include "aerell/support/unicode/utf8.h"
#include "aerell/support/range/ranges.h"
#include "aerell/support/unicode/unicode_ranges.h"

const bool ascii_whitespace[128] = {
    ['\t'] = true, ['\n'] = true, ['\v'] = true, ['\f'] = true, ['\r'] = true, [' '] = true};

const bool ascii_xid_start[128] = {['A' ... 'Z'] = true, ['a' ... 'z'] = true, ['_'] = true};

const bool ascii_xid_continue[128] = {['0' ... '9'] = true, ['A' ... 'Z'] = true, ['a' ... 'z'] = true, ['_'] = true};

bool is_unicode_whitespace(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_whitespace[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, whitespace_ranges, whitespace_ranges_count);
}

bool is_xid_start(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_xid_start[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, xid_start_ranges, xid_start_ranges_count);
}

bool is_xid_continue(const unsigned char c, size_t* ba)
{
    if(c < 128)
    {
        *ba = 1;
        return ascii_xid_continue[c];
    }

    *ba = utf8_uchar_len(c);
    if(*ba == 0) return false;

    return is_in_uint32_ranges(c, xid_continue_ranges, xid_continue_ranges_count);
}