#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;



int main() {
  // Flush after every std::cout / std:cerr
  cout << unitbuf;
  cerr << unitbuf;

  while(true) {
    cout << "$ ";
    string commandInput;
    getline(cin, commandInput);
    istringstream commandStream(commandInput);
    string command;
    commandStream >> command;
    vector<string> args;
    while(commandStream) {
      string argument;
      commandStream >> argument;
      args.push_back(argument); 
    }
    if(command == "exit") {
      if(stoi(args[0]) == 0 || stoi(args[0]) == 1) 
        exit(stoi(args[0]));
      else 
        cout << "Invalid argument." << endl;
    }
    else {
      cout << command << ": not found" << endl;
    }
    
  }


  return 0;
}
