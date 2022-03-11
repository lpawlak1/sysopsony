//
// Created by lpawlak on 3/7/22.
//

#ifndef SYSOPY_LIB_ZAD1_H
#define SYSOPY_LIB_ZAD1_H

#define WRAPPED_ARR
struct entry {
    int size;
    char* arr;
};

struct wrapped_arr {
    int size;
    struct entry** arr;
    char** filenames;
    int size_filenames;
};


void create_table(int size);
void count(char** filenames, int len);
int read_table_from_file();
void free_table();
void remove_block(int index);

#endif //SYSOPY_LIB_ZAD1_H
