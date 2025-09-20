/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef UNICODE_H
#define UNICODE_H

#include <stddef.h>

bool is_unicode_whitespace(const unsigned char c, size_t* ba);

bool is_xid_start(const unsigned char c, size_t* ba);

bool is_xid_continue(const unsigned char c, size_t* ba);

#endif