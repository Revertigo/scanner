//
// Created by dekel on 4/13/21.
//

#include "scanner.h"
#include <ctype.h>

#define HANDLE_DOUBLE_OPERATOR(second_op, tt, text) \
    {                                                \
        if(inputFile.peek() == second_op){ \
            nextChar();\
            return shared_ptr<Token> (new Token(tt, text));\
        }else{\
            goto single_char;\
        }                                               \
    }

shared_ptr<Token> Scanner:: nextToken()
{
    string text;

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
    };

    auto comment_line = [this]()
    {
        while (nextChar() && ch != '\n');
    };

    auto read_number = [this]()
    {
        string number = "";
        regex number_pattern(R"([0-9Ee+\-.])"); //On whitespace regex won't match
        while(nextChar() && regex_match(string(1,ch), number_pattern)){
            number += ch;
        }
        inputFile.unget();//return last read character to the file

        return number;
    };

    auto read_char = [this]()
    {
        string ch_str = "";
        if(nextChar()){
            ch_str = ch;
        }
        if(!(nextChar() && ch == '\'')){
            ch_str = "";
        }

        return ch_str;
    };

    auto read_string = [this]()
    {
        string str = "";
        while(nextChar() && ch != '\"'){
            str += ch;
        }

        return str;
    };

    auto read_word = [this]()
    {
        string word = "";
        regex word_pattern(R"([\w\d])"); //On whitespace regex won't match
        while(nextChar() && regex_match(string(1,ch), word_pattern)){
            word += ch;
        }
        inputFile.unget();//return last read character to the file

        return word;
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
            case '+' :
                HANDLE_DOUBLE_OPERATOR('+', INC_OP, "++");
            case '-' :
                if(inputFile.peek() == '-'){
                    nextChar();
                    return shared_ptr<Token> (new Token(DEC_OP, "--"));
                }else if(inputFile.peek() == '>'){
                    nextChar();
                    return shared_ptr<Token> (new Token(PTR_OP, "->"));
                }
                else{
                    goto single_char;
                }
            case '&' :
                HANDLE_DOUBLE_OPERATOR('&', AND_OP, "&&");
            case '|' :
                HANDLE_DOUBLE_OPERATOR('|', OR_OP, "||");
            case '<' :
                HANDLE_DOUBLE_OPERATOR('=', LE_OP, "<=");
            case '>' :
                HANDLE_DOUBLE_OPERATOR('=', GE_OP, ">=");
            case '=' :
                HANDLE_DOUBLE_OPERATOR('=', EQ_OP, "==");
            case '!' :
                HANDLE_DOUBLE_OPERATOR('=', NE_OP, "!=");

            case ';' :
            case '{' :
            case '}' :
            case ',' :
            case ':' :
            case '(' :
            case ')' :
            case '[' :
            case ']' :
            case '.' :
            case '~' :
            case '*' :
            case '/' :
            case '%' :
            case '^' :
            case '?' :
single_char:
                return shared_ptr<Token>
                        (new Token(static_cast<tokenType>(ch), string(1, ch)));

        }

        //Scan for number
        if(isdigit(ch)){
            text = ch + read_number();
            vector<regex> number_patterns { regex(R"(0)"), //0
                                            regex(R"([1-9](\d)*)"), //104
                                            regex(R"((\d)+[Ee][+-]?(\d)+)"),   //123E4
                                            regex(R"((\d)*\.(\d)+([Ee][+-]?(\d)+)?)"), //x.123
                                            regex(R"((\d)+\.(\d)*([Ee][+-]?(\d)+)?)")}; //123.x
            for(auto pattern: number_patterns){
                if(regex_match(text, pattern)){
                    return shared_ptr<Token>(new Token(CONSTANT, text));
                }
            }

            goto error;
        }

        //Scan for Char
        if(ch == '\''){
            text = '\'' + read_char() + '\'';
            regex char_pattern(R"(\'[^\']\')");

            if(regex_match(text, char_pattern)){
                return shared_ptr<Token>(new Token(CONSTANT, text));
            }

            goto error;
        }

        //Scan for String
        if(ch == '\"'){
            text = "\"" + read_string() + "\"";
            regex str_pattern(R"(\"[^\"]*\")");

            if(regex_match(text, str_pattern)){
                return shared_ptr<Token>(new Token(STRING_LITERAL, text));
            }

            goto error;
        }

        //Scan for words
        if(isalpha(ch)){
            text = ch + read_word();
            regex word_pattern(R"(\w(\w|\d)*)");
            //Check if token is valid word
            if(regex_match(text, word_pattern)){
                shared_ptr<Token> token = symTab.lookupToken(text);
                //If token not exists at symbol table
                if(token == nullptr){
                    shared_ptr<Token> var_token(new varToken(text));
                    var_token->add_line(lineno);
                    token = var_token;
                    symTab.insertToken(text, token);
                }
                else if(token->getType() == IDENTIFIER){
                    token->add_line(lineno);
                }

                return token;
            }
        }
    }

    //We reached the end of the file, no more tokens
    return nullptr;

error:
    return shared_ptr<Token>(new Token(ERROR, text));
}