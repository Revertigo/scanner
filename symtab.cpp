//
// Created by dekel on 4/13/21.
//
#include <sstream>
#include "symtab.h"

using namespace std;

shared_ptr<Token> SymbolTable::lookupToken(string text){
    return symMap[text];
}

void SymbolTable::insertToken(string text, shared_ptr<Token> tokenp){
    symMap[text] = tokenp;
}

void SymbolTable::xref(){
    map<string, shared_ptr<Token>>::iterator it;

    for (it = symMap.begin(); it != symMap.end(); it++){
        auto * variable = dynamic_cast<varToken*>(it->second.get());
        //Only if the token is a variable
        if(variable){
            shared_ptr<set<int>> lines = variable->getLines();
            string to_print = variable->getText() + "\t";
            //Adding line numbers
            for(auto line : *lines){
                to_print += (to_string(line) + " ");
            }
            cout << to_print << endl;
        }
    }
}

//Private functions
void SymbolTable::initReserved(){
    ifstream reserved_words("../resource/reserved.txt");

    if (!reserved_words) {
        cout << "Unable to open reserved words file " << endl;
        exit(1);
    }

    //Load all reserved words into the table
    string line;
    string text;
    int enum_type;
    while (getline(reserved_words, line)){
        istringstream iss(line);
        iss >> text >> enum_type; //Read line to variables
        shared_ptr<Token> token(new Token(static_cast<tokenType>(enum_type), text));
        insertToken(text, token);
    }
}
