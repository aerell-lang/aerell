#ifndef SOURCE_FILE_H
#define SOURCE_FILE_H

#include <stddef.h>

typedef struct SourceFile
{
    const char* file_path;
    const char* buffer;
    size_t buffer_size;
    size_t* line_offsets;
    size_t line_count;
} SourceFile;

SourceFile* source_file_create(const char* file_path);

void source_file_print(SourceFile* source_file);

void source_file_free(SourceFile* source_file);

#endif