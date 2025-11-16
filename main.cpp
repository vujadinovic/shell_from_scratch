#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <filesystem>




namespace fs = std::filesystem;
using namespace std;

void exit_shell(const vector<string>& args);
void echo(const vector<string>& args);
void type(const vector<string>& args);

unordered_map<string, void(*)(const vector<string>&)> BUILTINS = {
    {"exit", exit_shell},
    {"echo", echo},
    {"type", type}
};


void exit_shell(const vector<string>& args) {
  if(args.size() == 1)
    exit(0);
  else if(stoi(args[0]) == 0 || stoi(args[0]) == 1) 
    exit(stoi(args[0]));
  else 
    cout << "Invalid argument." << endl;
}

void echo(const vector<string>& args) {
  for(string arg : args){
    cout << arg << " ";
  }
  cout << endl;
}

vector<string> splitPath(string pathString) {
  #ifdef _WIN32
    const char PATH_SEPARATOR = ';';
  #else
    const char PATH_SEPARATOR = ':';
  #endif

    vector<string> directories;
    size_t start = 0;
    size_t position = 0;
    while((position = pathString.find(PATH_SEPARATOR, start)) != std::string::npos) {
      directories.push_back(pathString.substr(start, position - start));
      start = position + 1;
    }
    directories.push_back(pathString.substr(start));
    return directories;
}

bool isExecutable(fs::path& pathPath) {
  #ifdef _WIN32
    return fs::exists(pathPath) && (pathPath.extension() == ".exe" || 
                              pathPath.extension() == ".bat" || 
                              pathPath.extension() == ".cmd");
  #else
    return fs::exists(pathPath) && fs::is_regular_file(pathPath) && 
                                ((fs::status(pathPath).permissions() & fs::perms::owner_exec) != fs::perms::none);
  #endif
}

void type(const vector<string>& args) {
  if (args.empty()) {
        cout << "type: missing argument\n";
        return;
  }
  const string command = args[0];

  if (BUILTINS.find(command) != BUILTINS.end()) {
        cout << command << " is a shell builtin" << endl;
        return;
  }
  else {
    char* path = getenv("PATH");
    if(path == NULL) {
      cout << command << ": not found" << endl;  
      return;
    }

    string pathString(path);
    vector<string> directories = splitPath(pathString);

    for(const string& directory : directories) {
      fs::path fullPath = fs::path(directory) / command;
      #ifdef __WIN32
      vector<string> extensions = {".exe", ".bat", ".cmd"};
        for (string extension : extensions) {
            fs::path fullExtension = fullPath;
            fullExtension += extension;
            if (fs::exists(fullExtension)) {
                cout << command << " is " << fullExtension.string() << endl;
                return;
            }
        }
      #else
      if (isExecutable(fullPath)) {
            cout << command << " is " << fullPath.string() << endl;
            return;
      }
      #endif
    }
  

}


  cout << command << ": not found" << endl;  
}


int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;


  while(true) {
    cout << "$ ";

    // GET COMMAND INPUT
    string commandInput;
    getline(cin, commandInput);
    istringstream commandStream(commandInput);

    // PARSE COMMAND
    string command;
    commandStream >> command;

    // PARSE ARGUMENTS
    vector<string> args;
    while(commandStream) {
      string argument;
      commandStream >> argument;
      args.push_back(argument); 
    }

    // CHECK FUNCTION
    if(BUILTINS.find(command) == BUILTINS.end()) 
      cout << command << ": not found" << endl;
        
    else {
      BUILTINS[command](args);
    }
  }
  return 0;
}
