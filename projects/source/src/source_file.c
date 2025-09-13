#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "source/source_file.h"

SourceFile* source_file_create(const char* file_path)
{
    if(!file_path) return NULL;

    FILE* file = fopen(file_path, "rb");
    if(!file) return NULL;

    fseek(file, 0, SEEK_END);
    size_t buffer_size = ftell(file);

    char* buffer = malloc(buffer_size > 0 ? buffer_size + 1 : 1);
    if(!buffer)
    {
        fclose(file);
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    if(fread(buffer, 1, buffer_size, file) != buffer_size)
    {
        free(buffer);
        fclose(file);
        return NULL;
    }
    buffer[buffer_size] = '\0';

    fclose(file);

    size_t line_capacity = 8;
    size_t line_count = 0;
    size_t* line_offsets = malloc(line_capacity * sizeof(size_t));
    if(!line_offsets)
    {
        free(buffer);
        return NULL;
    }

    line_offsets[line_count++] = 0;

    for(size_t offset = 0; offset < buffer_size; offset++)
    {
        if(buffer[offset] != '\n') continue;
        if(line_count >= line_capacity)
        {
            line_capacity *= 2;
            size_t* line_offsets_tmp = realloc(line_offsets, line_capacity * sizeof(size_t));
            if(!line_offsets_tmp)
            {
                free(line_offsets);
                free(buffer);
                return NULL;
            }
            line_offsets = line_offsets_tmp;
        }
        line_offsets[line_count++] = offset + 1;
    }

    if(line_offsets[line_count - 1] != buffer_size)
    {
        if(line_count >= line_capacity)
        {
            line_capacity++;
            size_t* line_offsets_tmp = realloc(line_offsets, line_capacity * sizeof(size_t));
            if(!line_offsets_tmp)
            {
                free(line_offsets);
                free(buffer);
                return NULL;
            }
            line_offsets = line_offsets_tmp;
        }
        line_offsets[line_count++] = buffer_size;
    }

    size_t* line_offsets_tmp = realloc(line_offsets, line_count * sizeof(size_t));
    if(!line_offsets_tmp)
    {
        free(line_offsets);
        free(buffer);
        return NULL;
    }
    line_offsets = line_offsets_tmp;

    SourceFile* source_file = malloc(sizeof(SourceFile));
    if(!source_file)
    {
        free(buffer);
        free(line_offsets);
        return NULL;
    }

    source_file->file_path = strdup(file_path);
    if(!source_file->file_path)
    {
        free(buffer);
        free(line_offsets);
        free(source_file);
        return NULL;
    }

    source_file->buffer = buffer;
    source_file->buffer_size = buffer_size;
    source_file->line_offsets = line_offsets;
    source_file->line_count = line_count;

    return source_file;
}

void source_file_print(SourceFile* source_file)
{
    if(!source_file) return;
    printf("File Path: %s\n", source_file->file_path);
    printf("Buffer:\n%s\n", source_file->buffer);
    printf("Buffer Size: %zu\n", source_file->buffer_size);
    for(size_t i = 0; i < source_file->line_count; i++)
    {
        printf("Line: %zu, Offset: %zu\n", i, source_file->line_offsets[i]);
    }
    printf("Line Count: %zu\n", source_file->line_count);
}

void source_file_free(SourceFile* source_file)
{
    if(!source_file) return;
    if(source_file->file_path) free((char*)source_file->file_path);
    if(source_file->buffer) free((char*)source_file->buffer);
    source_file->buffer_size = 0;
    if(source_file->line_offsets) free(source_file->line_offsets);
    source_file->line_count = 0;
    free(source_file);
}