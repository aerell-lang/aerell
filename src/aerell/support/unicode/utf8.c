/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#include "aerell/support/unicode/utf8.h"

size_t utf8_uchar_len(const unsigned char c)
{
    if((c & 0x80) == 0) return 1;    // 0xxxxxxx
    if((c & 0xE0) == 0xC0) return 2; // 110xxxxx
    if((c & 0xF0) == 0xE0) return 3; // 1110xxxx
    if((c & 0xF8) == 0xF0) return 4; // 11110xxx
    return 0;                        // invalid
}