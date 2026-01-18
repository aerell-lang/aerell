// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#ifdef INCLUDE_AST
#pragma once
#endif

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#define INCLUDE_TOKENIZER
#include "aerell/tokenizer.c"

typedef struct
{
    TokenTag* tag;
    uint32_t* start;
} TokenList;

typedef enum
{
    NODE_TAG_ROOT,
    NODE_TAG_ADD,
    NODE_TAG_NUML
} NodeTag;

typedef union {
    uint32_t node;
    uint32_t token;
} NodeListData;

typedef struct
{
    NodeTag* tag;
    uint32_t* main_token;
    NodeListData* data;
    size_t capacity;
    size_t size;
} NodeList;

void node_list_free(NodeList* nl)
#ifdef INCLUDE_AST
    ;
#else
{
    if(nl->tag != NULL) free(nl->tag);
    if(nl->main_token != NULL) free(nl->main_token);
    if(nl->data != NULL) free(nl->data);
    nl->capacity = 0;
    nl->size = 0;
}
#endif

void node_list_append_assume_capacity(NodeList* nl, NodeTag tag, uint32_t main_token, NodeListData data)
#ifdef INCLUDE_AST
    ;
#else
{
    // Do something in here
    if(nl->size >= nl->capacity)
    {
        size_t new_capacity = (nl->capacity == 0) ? 2 : nl->capacity * 2;

        NodeTag* new_tag = realloc(nl->tag, new_capacity);
        assert(new_tag != NULL);
        if(new_tag == NULL) return;

        uint32_t* new_main_token = realloc(nl->main_token, new_capacity);
        assert(new_main_token != NULL);
        if(new_main_token == NULL)
        {
            free(new_tag);
            return;
        }

        NodeListData* new_data = realloc(nl->data, new_capacity);
        assert(new_data != NULL);
        if(new_data == NULL)
        {
            free(new_tag);
            free(new_main_token);
            return;
        }

        nl->capacity = new_capacity;
        nl->tag = new_tag;
        nl->main_token = new_main_token;
        nl->data = new_data;
    }

    nl->tag[nl->size] = tag;
    nl->main_token[nl->size] = main_token;
    nl->data[nl->size] = data;

    nl->size += 1;
}
#endif
