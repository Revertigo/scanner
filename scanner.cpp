//
// Created by dekel on 4/13/21.
//

/*
 */
#include "scanner.h"
#include <ctype.h>

shared_ptr<Token> Scanner:: nextToken()
{
    string text = "";

    // passing this in order to access class's methods
    auto comment = [this]()
    {
        char c, prev = 0;
        while (nextChar()){
            c = ch;
            if (c == '/' && prev == '*'){
                return;
            }
            prev = c;
        }
        //TODO:Handle unterminated comment
    };

    auto comment_line = [this]()
    {
        while (nextChar()){
            if (ch == '\n') {
                return;
            }
        }
    };

    auto reserved_word = [this]()
    {
        string text = "";
        while (nextChar() && ch != ' ');

    };

    auto read_number = [this]()
    {
        string number = "";
        regex number_pattern(R"([0-9Ee+-\.])"); //On whitespace regex won't match
        while(nextChar() && regex_match(string(1,ch), number_pattern)){
            number += ch;
        }
        //TODO:Handle EOF

        return number;
    };

    auto read_char = [this]()
    {
        string ch_str = "";
        while(nextChar() && ch != '\''){
            ch_str += ch;
        }
        //TODO:Handle EOF
        ch_str += ch;//Add last '

        return ch_str;
    };

    while(nextChar()) {
        //Check for start of comment /*
        if(ch == '/'){
            if(!nextChar()){
                return nullptr;
            }
            if(ch == '*'){
                //Current state is inside /* comment
                comment();
            }
            if(ch == '/'){
                //Current state is inside line comment (//)
                comment_line();
            }
        }

        if (ch == ' ' || ch == '\r' || ch == '\n') {
            continue;
        }

        switch (ch) { // each character represents itself
            case ';' :
            case '{' :
            case '}' :
            case ',' :
            case ':' :
            case '(' :
            case ')' :
            case '[' :
            case ']' :
            case '~' :
            case '*' :
            case '%' :
            case '^' :
            case '?' :
            case '/' :
                return shared_ptr<Token>
                        (new Token(static_cast<tokenType>(ch), string(1, ch)));
        }

        //Scan for number
        if(isdigit(ch)){
            text = ch + read_number();
            vector<regex> number_patterns { regex(R"([1-9](\d)*)"), //10
                                            regex(R"((\d)+[Ee][+-]?(\d)+)"),   //123E4
                                            regex(R"((\d)*\.(\d)+([Ee][+-]?(\d)+)?)"), //x.123
                                            regex(R"((\d)+\.(\d)*([Ee][+-]?(\d)+)?)")}; //123.x}; //'a'
            for(auto pattern: number_patterns){
                if(regex_match(text, pattern)){
                    return shared_ptr<Token>(new Token(CONSTANT, text));
                }
            }

            goto error;
        }

        if(ch == '\''){
            text = '\'' + read_char();
            regex char_pattern("\\'[^\\']\\'");

            if(regex_match(text, char_pattern)){
                return shared_ptr<Token>(new Token(CONSTANT, text));
            }

            goto error;
        }

        //Scan for reserved word
    }
    //We reached the end of the file, no more tokens
    return nullptr;

error:
    return shared_ptr<Token>(new Token(ERROR, text));
}