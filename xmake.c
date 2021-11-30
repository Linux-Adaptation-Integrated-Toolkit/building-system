#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

#include "utils.h"

int main() {
  
  char* xmake_exe;
  if(getenv("XMAKE_EXE")==NULL && getenv("WALI_XMAKE")==NULL) {
    printf("cant find xmake\n");
    return -1;
  }
  if(getenv("WALI_XMAKE")==NULL && getenv("XMAKE_EXE")!=NULL) {
    xmake_exe = c_string_merge(2, getenv("XMAKE_EXE"), "");
  }
  if(getenv("WALI_XMAKE")!=NULL && getenv("XMAKE_EXE")!=NULL) {
    xmake_exe = c_string_merge(2, getenv("WALI_XMAKE"), "/xmake");
  }
  if(getenv("WALI_XMAKE")!=NULL && getenv("XMAKE_EXE")==NULL) {
    xmake_exe = c_string_merge(2, getenv("WALI_XMAKE"), "/xmake");
  }  
  
  if(getenv("WALI_XMAKE_SCRIPT") == NULL) {
    printf("cant find xmake lua script, maybe you should set the WALI_XMAKE_SCRIPT environment.\n");
    setenv("WALI_XMAKE_SCRIPT","/usr/lib/wali/xmake_script",0);
  }

  char* command_line =  c_string_merge(4, xmake_exe, " lua ", getenv("WALI_XMAKE_SCRIPT"), "/info.lua");
  char* clear_cache = c_string_merge(2, xmake_exe, " f -c");

  /*
   * first clear the xmake cache.
  */
  FILE* xmake_ccfp = popen(clear_cache, "r");
  pclose(xmake_ccfp);
  //no need this var in heap.
  c_string_free(clear_cache);

  /*
   * execute command_line in tty, then get return to a json, finnaly use apt to install.
  */  
  FILE* fp = popen(command_line, "r");
  //no need for tow var in heap.
  c_string_free(command_line);
  c_string_free(xmake_exe);
  char line[4096];

	if (fp == NULL) {
		printf("Popen Error!\n");
		return -2;
  }

  if(fgets(line, 4096, fp) == NULL) {
    printf("cant get dependence list, is there a xmake.lua file?\n");
    return -3;
  }
  else {
    //parse the terminal output as a json.
    cJSON* jsonroot = cJSON_Parse(line);
    if(!jsonroot) {
      printf("Error before: [%s], xmake script generate an invalid return value.\nconsider to take a new issue at https://github.com/Linux-Adaptation-Integrated-Toolkit/building-system\n",cJSON_GetErrorPtr());
      return -4;
    }
    unsigned jsonsize = cJSON_GetArraySize(jsonroot);
    if(jsonsize == 0) {
      printf("maybe there is none valid 'add_requires' element in xmake.lua\n");
      return -5;
    }
    char* apt_showsrc_param = "";
    for(unsigned i = 0; i < jsonsize; ++i){
      cJSON* item = cJSON_GetArrayItem(jsonroot, i);
      printf("dependence = %s, installed = %s, optional = %s\n", item->string, cJSON_PrintUnformatted(cJSON_GetObjectItem(item, "installed")), cJSON_PrintUnformatted(cJSON_GetObjectItem(item, "optional")));
      //此处使用popen执行apt get，后面会替换使用libapt
      char* apt_cmd = c_string_merge(3, "apt showsrc ", item->string, " 2>/dev/null | grep 'Binary:' |sed s/[[:space:]]//g | awk -F ':' '{print $2}'");
      FILE* apt_fp = popen(apt_cmd, "r");
      c_string_free(apt_cmd);
      char apt_instr[4096];
      fgets(apt_instr, 4096, apt_fp);
      if(apt_instr[strlen(apt_instr)-1] == '\n') {
        apt_instr[strlen(apt_instr)-1] = '\0';
      }
      char* apt_out_item = strtok(apt_instr, ",");
      apt_showsrc_param = c_string_merge(3, apt_showsrc_param, " ", apt_out_item);
      while(apt_out_item != NULL) {
        apt_out_item = strtok(NULL, ",");
        if(apt_out_item == NULL) {
          break;
        }
        apt_showsrc_param = c_string_merge(3, apt_showsrc_param, " ", apt_out_item);
      }
      pclose(apt_fp);
    }
    char* apt_cmd = c_string_merge(3, "pkexec apt -y install", apt_showsrc_param, " 2>/dev/null");
    FILE* apt_fp = popen(apt_cmd, "r");
    c_string_free(apt_cmd);
    pclose(apt_fp);
    c_string_free(apt_showsrc_param);
  }
//apt showsrc cjson 2>/dev/null | grep "Binary:" |sed s/[[:space:]]//g | awk -F ":" '{print $2}'
  pclose(fp);

  return 0;
}