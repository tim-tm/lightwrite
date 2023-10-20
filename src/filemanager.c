#include "filemanager.h"
#include "lassert.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Collect all files in current dir
static void fileman_collect_files(File_Manager* man) {
    LASSERT(man);
    LASSERT(man->files);

    while ((man->direntry = readdir(man->dir)) != NULL) {
        struct stat stbuf;
        if (stat(man->direntry->d_name, &stbuf) == -1) {
            LOG_ERROR("Failed to stat %s", man->direntry->d_name);
            continue;
        }
        
        // TODO: Handle subdirectories
        if ((stbuf.st_mode & S_IFMT) == S_IFDIR) continue;

        if (man->size >= man->capacity) {
            man->capacity *= 2;
            man->files = realloc(man->files, man->capacity * sizeof(File));
        }    
        
        strcpy(man->files[man->size].name, man->direntry->d_name);
        
        // Create a ptr for reading
        man->files[man->size].ptr = fopen(man->files[man->size].name, "r");
        man->size++;
    }
}

void fileman_init(File_Manager* man) {
    LASSERT(man);
    man->capacity = FILEMAN_DEFAULT_CAPACITY;
    man->size = 0;
    man->files = calloc(man->capacity, sizeof(File));
    LASSERT(man->files);

    man->dir = opendir(".");
    LASSERT(man->dir);
    fileman_collect_files(man);
}

void fileman_destroy(File_Manager* man) {
    if (man) {
        if (man->files) {
            for (size_t i = 0; i <= man->size; ++i) {
                if (man->files[i].ptr) {
                    fclose(man->files[i].ptr);
                }
            }
            free(man->files);
        }
    }
}

bool fileman_create(File_Manager* man, const char* filename) {
    LASSERT(man);
    
    // Do not use LASSERT macro here since the editor should not quit if the filename is empty, instead handle it by drawing some kind of error
    if (!filename) {
        LOG_ERROR("Invalid filename.");
        return false;
    }

    // TODO: Here should probably be some kind of directory checking.
    
    for (size_t i = 0; i <= man->size; ++i) {
        if (!strcmp(man->files[i].name, filename)) {
            LOG_ERROR("File %s already exists!", filename);
            return false;
        }
    }

    // this will create the file
    FILE* fp = fopen(filename, "w+");
    if (!fp) {
        LOG_ERROR("Failed to create: %s", filename);
        return false;
    }

    // Add the file to the cached files
    if (man->size >= man->capacity) {
        man->capacity *= 2;
        man->files = realloc(man->files, man->capacity * sizeof(File));
    }

    strcpy(man->files[man->size].name, filename);
    man->files[man->size].ptr = fp;

    man->size++;
    return true;
}
