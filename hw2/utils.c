#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
	You are free to use any data type you like in this file.
    However, other than the five functions declared in "utils.h",
    DO NOT create any other functions.
	
*/

int cmpr_int(void *a, void *b) {
	int iA = *(int *)a;
    int iB = *(int *)b;

    if(iA == iB){
        return 0;
    }
    if (iA > iB){
        return 1;
    }
    else{
        return -1;
    }
}

int cmpr_float(void *a, void *b) {
	float fA = *(float *)a;
    float fB = *(float *)b;

    if(fA == fB){
        return 0;
    }
    if (fA > fB){
        return 1;
    }
    else{
        return -1;
    }
}

void print_int(void *i) {
	int* pointer = (int*)i;
	int val = *pointer;
	printf("%d\n", val);
}

void print_float(void *f) {
	float* pointer = (float*)f;
	float val = *pointer;
	printf("%f\n", val);
}


void* read_array(char* filename, char* format, size_t* len) {
    FILE *fi;
    fi = fopen(filename, "r");
    if (fi == NULL) {
        fprintf(stderr, "File failed to open");
        exit(1);
    }

    char buffer[1000];

    void* array = NULL;
    int i = 0;

    if (strcmp(format, "%d") == 0) {
        array = malloc(*len * sizeof(int));
    }
    else if (strcmp(format, "%f") == 0){
        array = malloc(*len * sizeof(float));
    }
	else {
		printf("Invalid format");
		exit(1);
	}

    while (fgets(buffer, sizeof(buffer), fi) != NULL) {
        if (strcmp(format, "%d") == 0) {
            int value;
            sscanf(buffer, "%d", &value);
            ((int*)array)[i] = value;
        } else {
            float value;
            sscanf(buffer, "%f", &value);
			// printf("%s", buffer);
			// printf("%f", value);
            ((float*)array)[i] = value;
        }
        i++;
        (*len)++;
    }
	fclose(fi);
    return array;
	free(array);
}

// int main(){
	
// }
