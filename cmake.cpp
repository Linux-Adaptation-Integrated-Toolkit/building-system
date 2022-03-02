#include <regex>
#include <vector>

#if __GNUC__ <= 5
    #error your compiler cannot support C++17 standard. we need at least GCC 6
#elif (__GNUC__ > 5) && (__GNUC__ < 11)
    #include <experimental/filesystem>
#else
    #include <filesystem>
#endif

#include <fstream>
#include <iostream>
#include <string>

using namespace std;
#if (__GNUC__ > 6) && (__GNUC__ < 11)
    namespace filesystem = experimental::filesystem;
#endif

//remove space with cpp API
string removeSpace(string str) {
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  //res.assign(res.begin(), res.end());
  return str;
}
//remove space with regex
string removeSpace2(string str) {
  string p = "\x20";
  regex express(p);
  regex_replace(str, express, '\0');
  return str;
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    cerr << "please offer path of project directory (only)" << endl;
    return 1;
  }
  filesystem::path CmakeRootPath = argv[1];
  if(!filesystem::exists(CmakeRootPath)) {
    cerr << "offer a correct path, and make sure it is a directory" << endl;
    return 2;
  }
  if(!filesystem::is_directory(CmakeRootPath)) {
    cerr << "please offer a path of directory, not a file" << endl;
    return 2;
  }
  string Cmakefile = (string)CmakeRootPath + (string)"/CMakeLists.txt";
  if(!filesystem::exists(Cmakefile)) {
    cerr << "the path you have offerd doesn't contain CMakeLists.txt in top directory." << endl;
    return 2;
  }
  
  ifstream fin(Cmakefile, ios::in);
  char line[1024] = {0};
  while(fin.getline(line, sizeof(line))) {
    string linestr = removeSpace(line);
    if(linestr.empty()) {
      continue;
    }
    if(linestr.at(0) == '#') {
      continue;
    }
    string p = "find_package(";
  }
  fin.clear();
  fin.close();

  return 0;
}