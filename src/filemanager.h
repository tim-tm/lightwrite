#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>

#define FILEMAN_DEFAULT_CAPACITY 16

// NOTE: This macro needs string.h and stdlib.h
#define FILEMAN_PUSH(man, file, filename)                                   \
    if (man->size >= man->capacity) {                                       \
        man->capacity *= 2;                                                 \
        man->files = realloc(man->files, man->capacity * sizeof(File));     \
    }                                                                       \
                                                                            \
    strcpy(man->files[man->size-1].name, filename);                         \
    man->files[man->size-1].ptr = file;                                     \
    man->size++;                                                            \

typedef struct _File_ {
    FILE* ptr;
    char name[256];
} File;

typedef struct _File_Manager_ {
    File* files;
    struct dirent* direntry;
    DIR* dir;
        
    size_t size;
    size_t capacity;
} File_Manager;

void fileman_init(File_Manager* man);
void fileman_destroy(File_Manager* man);

bool fileman_create(File_Manager* man, const char* filename);
void fileman_push(File_Manager* man, FILE* file, const char* filename);

#endif // !_FILEMANAGER_H_
