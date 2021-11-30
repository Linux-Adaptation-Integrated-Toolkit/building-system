#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Combine multiple strings
//set argc to number of strings
char* c_string_merge(int argc, const char* argv, ...) {
    va_list list;
    va_start(list, argv);
    char* result = (char*)malloc(strlen(argv) + 1);
    strcpy(result, argv);
    for(int i =1; i < argc; ++i){
        char* newstr = va_arg(list, char*);
        result = (char*)realloc(result, strlen(result) + strlen(newstr));
        strcat(result, newstr);
    }
    strcat(result, "\0");
    va_end(list);

    return result;
}

//Reclaim the memory and point the pointer to NULL
void c_string_free(char* a) {
    free(a);
    a = NULL;
}

//demo
/*
int main() {
    char* a = c_string_merge(3, "123", "456", "456");
    printf("%s\n", a);
    c_string_free(a);
    return 0;
}
*/