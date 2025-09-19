/*
 * Copyright (c) 2025 The Aerell Authors
 * All rights reserved — BSD 3-Clause License
 * See LICENSE file for details.
 */

#ifndef LEXER_H
#define LEXER_H

#include "lexer/token/tokens.h"
#include "source/source_file.h"

tokens_t* lexer(source_file_t* source_file);

#endif