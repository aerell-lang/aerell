#ifndef SOURCE_H
#define SOURCE_H

typedef struct SourceFile SourceFile;

SourceFile* source_file_create(const char* file_path);

void source_file_print(SourceFile* source_file);

void source_file_free(SourceFile* source_file);

#endif