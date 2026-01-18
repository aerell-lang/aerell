// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifdef INCLUDE_PARSE
#pragma once
#endif

#ifndef INCLUDE_PARSE
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#endif

#define INCLUDE_AST
#include "aerell/ast.c"
#undef INCLUDE_AST

typedef struct
{
    const char* source;
    TokenList tokens;
    uint32_t token_index;
    NodeList nodes;
} Parse;

void parse_root(Parse* p)
#ifdef INCLUDE_PARSE
    ;
#else
{
    assert(p != NULL && "p is null");
    if(p == NULL) return;

    // Root node must be index 0
    NodeListData data;
    data.node = 0;

    printf("This is nodes size before append: %zu\n", p->nodes.size);
    node_list_append_assume_capacity(&p->nodes, NODE_TAG_ROOT, 0, data);
    printf("This is nodes size after append: %zu\n", p->nodes.size);
    node_list_free(&p->nodes);
}
#endif
