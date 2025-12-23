/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#if defined(_WIN32)
#include <windows.h>
#endif

void exit(int code)
{
#if defined(_WIN32)
    ExitProcess(code);
#else
#error "Only Win supported"
#endif
}