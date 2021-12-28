#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

struct lswap {
  unsigned linenum;
  string type; 
};

/*
  get the dependent list through substitude the required words to null,
  after getting it, return the substitude back. 
*/

int main () {
  string filepath = "./CMakeLists.txt";
  vector<lswap> lines;
  string line;
  int linenum = 0;
  string type;
  lswap lineswap;

  string p_required = "REQUIRED";
  string p_find_package = "find_package";

  ifstream input_file(filepath);
  if (!input_file.is_open()) {
      cerr << "Could not open the file - '"
            << filepath << "'" << endl;
      return -1;
  }

  //replace the code
  while(getline(input_file, line)) {
    linenum++;
    lineswap.linenum = linenum;
    regex r(p_find_package);
    bool regex_find_package = regex_search(line, r);
    if (regex_find_package) {
      lineswap.type = "optional";
      regex p(p_required);
      bool regex_required = regex_search(line, p);
      if(regex_required) {
        lineswap.type = "required";
        regex_replace(line, p_required, "");
      }
      lines.push_back(lineswap);
    }
  }
  //make a new directory named build, then cd into execute cmake command
  
  //recover the original code.
  for (const auto &i : lines) {
    cout << i.linenum << i.type << endl;
  }

  input_file.close();
  return 0;
}