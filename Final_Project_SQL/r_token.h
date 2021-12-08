#ifndef R_TOKEN_H
#define R_TOKEN_H

#include <iostream>
#include "../../B Tree/bplusmap/map.h"
using namespace std;



class r_Token
{
public:
    //CTOR
    r_Token();
    //Given a string for the token
    r_Token(string token);
    //Given a string and a type
    r_Token(string token, int type);


    bool is_Valid();

    int getType();
    int getPrecedence();

    string token();
    //Types
    //0 for symbols
   // Logical Operators
    //1 OR
    //2 AND
    //3 NOT
   // Relational operators
    //4 =
    //5 >
    //6 >=
    //7 <
    //8 <=
    //9  (
    //10 )

    //Output
    friend ostream& operator << (ostream& outs, r_Token* rhs)
    {
        outs << rhs->token_str;

        return outs;
    }



private:
    //r_Token Type
    int _type;
    //Types
    //0 for symbols
   // Logical Operators
    //1 OR
    //2 AND
    //3 NOT
   // Relational operators
    //4 =
    //5 >
    //6 >=
    //7 <
    //8 <=
   // Parenthesis
    //9 ( left parenthesis
    //10 ) right parenthesis

    //Token precedence
    int _precedence;
    //Precedence
    //0 for symbols
   // Parenthesis
    //5 ( Left parenthesis
    //6 ) Right Parenthesis
   // Logical Operators
    //4 OR
    //3 AND
    //2 NOT
   // Relational operators
    //1 =
    //1 >
    //1 >=
    //1 <
    //1 <=

    void build_types();

    //Map of tokens and their types
    Map<string, int> token_types;

    //r_Token string
    string token_str;


};

#endif // R_TOKEN_H
