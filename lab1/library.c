#define  _GNU_SOURCE
#include "library.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include "string.h"

struct wrapped_arr * wa = NULL;

char global_arr[1000000];

void create_table(int size){
	if (size <= 0) {
		fprintf(stderr, "[create_table] Wrong block size\n");
		return;
	}

	if (wa != NULL) {
		fprintf(stderr, "[create_table] Block already created\n");
		return;
	}
    
    wa = (struct wrapped_arr *)calloc(1, sizeof(struct wrapped_arr));
    wa->size = size;
    if (size != 0){
        wa->arr = (struct entry**)calloc(size, sizeof(struct entry*));
    }

	if (wa->arr == NULL) {
		fprintf(stderr, "[create_table] calloc failed\n");
		exit(1);
	}
}

void count(char** filenames, int len){
    if (len == 0) {
        return;
    }
    
    const char* prior = "wc "; 
    const char* prior2 = "> temp";
    global_arr[0] = '\0'; // czyszczenie str fastly
    strcat(global_arr, prior);
    for (int i = 0, n = len;i < n;i++) {
        strcat(global_arr, filenames[i]);
        strcat(global_arr, " ");
    }
    strcat(global_arr, prior2);
    
    system(global_arr); //uruchom w system komende

    fprintf(stdout, "[count], counted -> %s", global_arr);
}

int find_next_empty_block() {
    for (int i = 0; i < wa->size; i++){
        if (wa->arr[i] == NULL){
            return i;
        }
    }
    fprintf(stderr, "[find_next_empty_block] No free blocks left :(");
    exit(4);
}

long get_file_size(FILE* fp) {
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return size;
}

int read_table_from_file(){
    FILE *fp;
    int i = 0;
    
    fp = fopen("temp", "r");
    if (ferror(fp)) {
        fprintf(stderr, "[read_table_from_file] Error with file opening");
    }

    long size = get_file_size(fp);


    i = find_next_empty_block();

    wa->arr[i] = calloc(1, sizeof(struct entry));
    if (wa->arr[i] == NULL){
        fprintf(stderr, "[read_table_from_file] Unable to calloc entry_1");
    }

    wa->arr[i]->size = size;
    wa->arr[i]->arr = calloc(size + 1, sizeof(char));
    if (wa->arr[i]->arr == NULL){
        fprintf(stderr, "[read_table_from_file] Unable to calloc entry_1->arr");
    }
    
    fread(wa->arr[i]->arr, 1, size, fp);
    wa->arr[i]->arr[size] = '\0';
    
    
    fclose(fp);
    return i;
}


void free_table(){
	if (wa == NULL || wa->arr== NULL) {
		return;
	}

	struct entry* entry_ = NULL;
	for (int i = 0; i < wa->size; i++) {
		entry_ = wa->arr[i];

		if (entry_ == NULL) {
			continue;
		}

		free(entry_->arr);
		free(entry_);
	}

	free(wa->arr);
	free(wa);
}

/**
 * Removes block (3 ints) from tab
 * @param tab tab to remove from, needs to be pointer of pointer to reassign after realloc
 * @param size size of tab
 * @param index specifies at what index this should be deleted
 * @return 0 if no errors, 1 if index is too large
 */
void remove_block(int index){
    if (index >= wa->size){
        fprintf(stderr, "[remove_block] Index too big");
        exit(4);
    }
    free(wa->arr[index]->arr);
    free(wa->arr[index]);
    int i = index;
    for(;i < wa->size-1; i++){
        wa->arr[i] = wa->arr[i+1];
    }
    wa->size--;
}
