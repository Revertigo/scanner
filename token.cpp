//
// Created by dekel on 4/13/21.
//

#include "token.h"
#include "tokenstrings.h"

void Token::printToken(){
    string token = text;
    if(type >= IDENTIFIER){
        token = tokenStrings[type - IDENTIFIER];
    }

    cout << token << "\t" << text << endl;
}

void varToken::add_line(int l){
    lines->insert(l);
}

shared_ptr<set<int>> varToken::getLines(){
    return lines;
}


