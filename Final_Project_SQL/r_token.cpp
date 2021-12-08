#include "r_token.h"

//Default constructor
r_Token::r_Token()
{
    _type = -1;
    token_str = "Default Token";
    _precedence = -1;
    build_types();
}

//Given just the token string
r_Token::r_Token(string token)
{
    const bool DEBUG = false;
    //Initializes token str to given string
    token_str = token;
    build_types();

    if (DEBUG)
    {
        cout << "---TOKEN(string) CONSTRUCTOR---" << endl;
        cout << "Token: " << token_str << endl;
        if (token_types.contains(token_str))
        {
            cout << "Contains" << endl;
        }
        else
        {
            cout << "Does not contain" << endl;
        }
    }

    //Sets type to token type based on map
    if (token_types.contains(token_str)) //The map contains all operators
                                            //map does not include symbols
    {
        _type = token_types[token_str];
    }
    else    //If not found in map then it initialize type to symbol
    {
        //Symbol (type 0)
        _type = 0;
    }

    //If symbol
    if (_type == 0)
    {
        _precedence = 0; //Meaningless b/c symbols don't need precedence
    }
    //If l_operator: OR
    else if (_type == 1)
    {
        _precedence = 1;    //Lowest precedence
    }
    //If l_operator: AND
    else if (_type == 2)
    {
        _precedence = 2;
    }
    //If l_operator: NOT
    else if (_type == 3)
    {
        _precedence = 3;
    }
    else if (_type == 4 || _type == 5 || _type == 6 || _type == 7
             || _type == 8)
    {
        _precedence = 4;    //All relational operators have the same prec.
    }
    //If LEFT PARENTHESIS
    else if (_type == 9) // (
    {
        _precedence = 5;
    }
    else if (_type == 10) // )
    {
        _precedence = 6;
    }

}

//Constructor takes in a string for token type and an int for type
r_Token::r_Token(string token, int type)
{
    //Initializes token string to given string
    token_str = token;
    //Initializes type to type
    _type = type;

    build_types();
    //If symbol
    if (_type == 0)
    {
        _precedence = 0; //Meaningless b/c symbols don't need precedence
    }
    //If l_operator: OR
    else if (_type == 1)
    {
        _precedence = 4;    //Highest precedence
    }
    //If l_operator: AND
    else if (_type == 2)
    {
        _precedence = 3;
    }
    //If l_operator: NOT
    else if (_type == 3)
    {
        _precedence = 2;
    }
    //Relationship operators (= <= < > >=)
    else if (_type == 4 || _type == 5 || _type == 6 || _type == 7
             || _type == 8)
    {
        _precedence = 1;    //All relational operators have the same prec.
    }
}


bool r_Token::is_Valid()
{
    //Returns true if the token type in the map is the same as the token str
    if (token_types.contains(token_str))
    {
         return (_type == token_types[token_str]);
    }
    else
    {
        //If not found in token type keys, then it should be a symbol
        return (_type == 0);
    }

}

//Returns Type
int r_Token::getType()
{
    return _type;
}

int r_Token::getPrecedence()
{
    return _precedence;
}

//Returns the token string
string r_Token::token()
{
    return token_str;
}

void r_Token::build_types()
{
    const bool DEBUG = false;
    string keys[10] = {"or", "and", "not", "=", ">", ">=", "<", "<=" ,
                       "(", ")"};

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

    for (size_t i = 0; i < 10; i++)
    {
        //We do not insert symbols b/c that is arbitrary
        //i + 2 b/c 'or' starts at 1
        token_types.insert(keys[i], i+1);
    }

    if (DEBUG)
    {
        cout << "--BUILD TYPES--" << endl;
        cout << "MAP OF TOKEN TYPES: " << endl;
        cout << token_types << endl;
    }


}
