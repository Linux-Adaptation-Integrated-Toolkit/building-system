#include "stdio.h"
#include "stdlib.h"

int main() {
  char* command_line = "xmake require --info";
  FILE* fp = popen(command_line, "r");
  char line[256];

	if (fp == NULL) {
		printf("Popen Error!\n");
		return -2;
  }
  while(!feof(fp)) {
    if(fgets(line, 256, fp) != NULL) {
      printf("%s", line);
    }
  }
  return pclose(fp);
}