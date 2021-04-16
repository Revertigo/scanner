//
// Created by dekel on 4/13/21.
//
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

    string line;
    string delimiter = "\t";
    while (getline(reserved_words, line)){
        size_t pos = line.find(delimiter);
        string text = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        pos = line.find(delimiter);
        string enum_type = line.substr(0, pos);
        auto token_type = static_cast<tokenType>(stoi(enum_type));
        shared_ptr<Token> token(new Token(token_type, text));
        insertToken(text, token);
    }
}
