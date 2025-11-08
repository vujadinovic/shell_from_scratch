#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;


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


int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  unordered_map<string, void(*)(const vector<string>&)> BUILTINS = {
    {"exit", exit_shell},
    {"echo", echo}
  };

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
